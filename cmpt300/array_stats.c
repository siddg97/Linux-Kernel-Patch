#include "array_stats.h"
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/errno.h>

SYSCALL_DEFINE3(array_stats, struct array_stats*,stats, long *, data, long, size) {
	if(size <= 0){
		return -EINVAL;
	}

	// temp pointer to array items: temp -> data -> data[0]
	long* temp;
	long elem;

	long i = 0; // offset from  data pointer

	long min, max, sum;
	sum = 0;

	// i = 0 : data[0] -> elem 
	if(copy_from_user(&elem,data, sizeof(long)) != 0){
		return -EFAULT;
	}
	min = elem;	// minimum is data[0]
	max = elem;	// maximum is data[0]
	sum += elem;// sum is data[0]
	i++;			// i = 1;

	while(i < size){
		temp = data;
		temp += i;  // temp -> data[i] | i=1 on first iteration 
		if(copy_from_user(&elem,temp, sizeof(long)) != 0){
			return -EFAULT;
		}
		sum += elem; // update sum
		if(elem < min){ // check to update min
			min = elem;
		}
		if(elem > max){ // chech to update max
			max = elem;
		}
		i++;             // increment offset
	}

	// local stats variable
	struct array_stats s;
	// Assign members
	s.min = min;
	s.max = max;
	s.sum = sum;
	
	// print form kernel : DEBUGGING SECTION
	//printk("STATS: Min=%ld Max=%ld Sum=%ld\n",min,max,sum);
	
	// copy to user space into stats pointer
	if(copy_to_user(stats, &s, sizeof(struct array_stats)) != 0){
		return -EFAULT;
	}
	return 0;	// SUCCESS 

}