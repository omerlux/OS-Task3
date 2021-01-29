#include "types.h"
#include "stat.h"
#include "user.h"

#include "fcntl.h"

////passing command line arguments

 void theBestSignalHandler(int pid, int value){
	         printf (1, "new sig_handler: I am the best signal handler my pid %d from recipient_pid %d, value %d\n",getpid(),pid,value);
 }

int main(int argc, char **argv)
{
	printf(1, "========= Default sig_handler ==========\n");

	int pid = fork();
	
    if(pid == 0){
       sleep(3);
	   printf(1, "Marco has woken up...\n");
	   exit();
	}else if(pid < 0){
        printf (1, "This is fork failed\n");
	}
	printf(1, "Anna (pid %d) is sending Marco (pid %d) a signal...\n", getpid(), pid);
	sigsend(pid, 17);
	wait();
	printf(1, "Anna has done waiting for Marco...\n");	
	
	printf(1, "========== New sig_handler ===========\n");
	pid = fork();
	if(pid == 0){
		sigset((sig_handler)(&theBestSignalHandler));
		sleep(3);
	    printf(1, "Marco has woken up...\n");
	    exit();
    } else if(pid < 0){
        printf (1, "This is fork failed\n");
    }
	sleep(2);
	printf(1, "Anna (pid %d) is sending Marco (pid %d) a signal...\n", getpid(), pid);
	sigsend(pid, 17);
	wait();
	printf(1, "Anna has done waiting for Marco...\n");
	
	printf(1, "=========== END TESTING ============\n");
	
    exit();
}

