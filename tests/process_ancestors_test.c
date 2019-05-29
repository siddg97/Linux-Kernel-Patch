#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <stdlib.h>

#define ANCESTOR_NAME_LEN 16

struct process_info {
	long pid;                     /* Process ID */
	char name[ANCESTOR_NAME_LEN]; /* Program name of process */
	long state;                   /* Current process state */
	long uid;                     /* User ID of process owner */
	long nvcsw;                   /* # of voluntary context switches */
	long nivcsw;                  /* # of involuntary context switches */
	long num_children;            /* # of children processes */
	long num_siblings;            /* # of sibling processes */
};


int main(int argc, char* argv[]){
	struct process_info p[10];
	long num;
	printf("DIVING INTO KERNEL MODE\n\n");
	int test = syscall(342,p,10,&num);
	printf("RISING BACK TO USER MODE\n");
	printf("\n INFO ON %ld PROCESSES WRITTEN BY KERNEL\n",num);
	long i = 0;
	while(i < num){
		printf("\nProcess Info:\tPID: %ld\t NAME: %s\t STATE: %ld UID: %ld NVCSW: %ld NIVCSW: %ld #of Children: %ld #of Siblings: %ld\n", p[i].pid,p[i].name,p[i].state,p[i].uid,p[i].nvcsw,p[i].nivcsw,p[i].num_children,p[i].num_siblings);
		i++;
	}
	return 0;
}