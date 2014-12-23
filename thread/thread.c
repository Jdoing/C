#include<stdio.h>
#include<pthread.h>
#include<unistd.h>

static int run = 1;
static int retvalue;

int main(void){
    
    void *start_routine(void *);
    
    pthread_t pt;
    
    int ret = -1;
    int times = 5;
    int i = 0;
    int *ret_join = NULL;
    ret = pthread_create(&pt, NULL, (void *)start_routine, &run);
    
    if(ret !=0 )
    {
		printf("create thread fail!\n");
		return 1;
	}
	
	usleep(1);
	for(;i < times; ++i)
	{
		printf("the main thread print\n");
		usleep(100);
    }
    
    run = 0;
    
    pthread_join(pt, (void *)&ret_join);
    printf("thread return valueis: %d\n", *ret_join);
    
    return 0;
}

void *start_routine(void *arg){
	int *running = arg;
	printf("init routine finish, pass parameter is: %d\n", *running);
	
	while(*running){
		printf("sub thread is running\n");
		usleep(100);
	}
	
	printf("routine exit\n");
	retvalue = 8;
	pthread_exit((void *)&retvalue);
	
}
