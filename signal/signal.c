#include<signal.h>
#include<stdio.h>
typedef void (*sighandler_t)(int);

void sig_handle(int singo){
		
		if(SIGSTOP == singo){
			printf("receive signal SITSTOP\n");
		}else if(SIGKILL == singo){
			printf("receive signal SIGKILL\n");
		}else{
			printf("receive signal SIGINT\n");
		}
		
	return;
}	

int main(void)
{
	sighandler_t ret;
	
	ret = signal(SIGSTOP, sig_handle);
	if(SIG_ERR == ret){
		printf("signal SIGSTOP occur erro\n");
		return -1;
	}
	
	ret = signal(SIGKILL, sig_handle);
	if(SIG_ERR == ret){
		printf("signal SIGKILL occur erro\n");
		return -1;
	}
	while(1);
	return 0;
}
