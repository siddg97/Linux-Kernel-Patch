#include "process_ancestors.h"
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/errno.h>
#include <linux/sched.h>
#include <linux/types.h>

SYSCALL_DEFINE3(process_ancestors, struct process_info*, info_array, long, size, long*, num_filled) {
	if(size <= 0){
		return -EINVAL;
	}

	long count = 0; 			// to be written to num_filled
	long i = 0; 				//offset for info_array
	struct process_info p;		// Used to store in kernel before copying process info to userspace
	struct process_info* arrPtr;// use this to traverse through info_array[size]
	long cCount = 0;			// counting number of children
	long sCount = 0;			// counting number of siblings
	struct list_head* hChild;	// head of children circular linked list
	struct list_head* hSibling;	// head of sibling circular linked list
	struct list_head* temp;		// pointer used to traverse the linked lists
	struct list_head* tmp_head; // temp. header for printing info for children of process

	// Initialize fields to get for each process
	long pid, state, uid, nvcsw, nivcsw, num_children, num_siblings;

	// get current pointer (global variable)
	struct task_struct *cur_task = current;
	struct task_struct *ts;
	do{
		if(i > 0 && cur_task->parent != cur_task){							
			cur_task = cur_task->parent;
		}
		// Get all stats apart form num_children and num_siblings
		pid = cur_task->pid;
		strcpy(p.name, cur_task->comm);
		state = cur_task->state;
		uid = (long)((cur_task->cred)->uid.val);
		nvcsw = cur_task->nvcsw;
		nivcsw = cur_task->nivcsw;

		hChild = &(cur_task->children);	// head of children circular linked list
		hSibling = &(cur_task->sibling);	// head of sibling circular linked list

		// count number of children
		cCount = 0;
		list_for_each(temp, hChild) {
        	++cCount;
    	}
    	// Section for printing child info: [FUN Actibity in Assignment Description]
    	// list_for_each(tmp_head, hChild){
    	// 	ts = list_entry(tmp_head, struct task_struct, sibling);
    	// 	printk("Child info: \nPID: %ld\t\t Name:%s\t\n PPID: \t%ld \t\t PName: %s\n\n",ts->pid,ts->comm,ts->parent->pid,ts->parent->comm);
    	// }
		num_children = cCount;				// number of children
		
		// count number of siblings
		sCount = 0;
		list_for_each(temp, hSibling) {
        	++sCount;
    	}
    	if(sCount != 0){
    		--sCount;					// current process is also counted, hence , minus 1 to acomodate for siblings except current process
		}
		num_siblings = sCount;				// number of siblings

		// populate fields of p (of type struct process_info)
		p.pid = pid;
		p.state = state;
		p.uid = uid;
		p.nvcsw = nvcsw;
		p.nivcsw = nivcsw;
		p.num_children = num_children;
		p.num_siblings = num_siblings;
		
		// Initialize arrPtr with appropiate offset
		arrPtr = info_array;				// points to info_array[0]
		arrPtr += i; 						// add offset to arrPtr => points to info_array[i]

		// write p to info_array[i]
		if(copy_to_user(arrPtr,&p,sizeof(struct process_info)) != 0){
			return -EFAULT;
		}
		i++;								// increment offset for info_array[]
		count++;							// increment the number of process stats written to info_array[]
		
		if(i >= size){						// break if array size is less than number of ancestors of current process
			break;
		}
		
	}while(cur_task->parent != cur_task); 

	// write to num_filled
	if(copy_to_user(num_filled,&count,sizeof(long)) != 0){
		return -EFAULT;
	}

	return 0;
}