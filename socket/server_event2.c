#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include <event.h>
#include <errno.h> 
#include <fcntl.h>  
#include <signal.h>
#include <event2/bufferevent.h>  

#define PORT 8888
#define BUF_SIZE 200
#define BACKLOG 10
#define FD_MAX_SIZE 3
#define MAX_EVENTS 3
#define MAX_LINE 256 
typedef void (*sighandler_t)(int);

int listenfd, cfd, epfd;
int revlen;
int yes;
char buffer[BUF_SIZE];
struct sockaddr_in server_addr;
struct sockaddr_in client_addr;

sighandler_t ret;

socklen_t addrlen = sizeof(struct sockaddr_in);

	struct event_base *base =NULL;
    struct event *listener_event =NULL;

void sig_handle(int singo){
		
		if(SIGSTOP == singo){
			printf("receive signal SITSTOP\n");
			exit(0);
		}else if(SIGKILL == singo){
			printf("receive signal SIGKILL\n");
			exit(0);
		}else{
			printf("receive signal SIGINT\n");
			exit(0);
		}
	close(epfd);	
	return;
}
   
void start_server(){
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if(listenfd < 0)
    {
		perror("socket error!\n");
		exit(1);
    }
	  
	evutil_make_listen_socket_reuseable(listenfd);
    
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(PORT);

	if(bind(listenfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
		perror("bind error");  
        exit(1); 
	}

	if(listen(listenfd, BACKLOG) < 0){      
		perror("listen error");  
        exit(1);  
    }
    
	//把socket设置为非阻塞方式  
	evutil_make_socket_nonblocking(listenfd);
	
	printf("server is up...\n");
}

void write_cb(struct bufferevent *bev, void *arg) {}

void read_cb(struct bufferevent *bev, void *arg)  
{  
 
    char line[MAX_LINE+1];  
	int n;
	evutil_socket_t fd = bufferevent_getfd(bev);

	while(n = bufferevent_read(bev, line, MAX_LINE), n > 0) {
		line[n] = '\0';
		printf("fd=%u, read line: %s\n", fd, line);
		bufferevent_write(bev, line, n);  
	}  
}  

void error_cb(struct bufferevent *bev, short event, void *arg)  
{  
	evutil_socket_t fd = bufferevent_getfd(bev);
	printf("fd = %u, ", fd);  
	if (event & BEV_EVENT_TIMEOUT) {
		printf("Timed out\n"); //if bufferevent_set_timeouts() called  
	}
	else if (event & BEV_EVENT_EOF) {
		printf("connection closed\n");  
	}
	else if (event & BEV_EVENT_ERROR){
		printf("some other error\n");  
	}
	
	bufferevent_free(bev);  
} 

void do_accept(evutil_socket_t fd, short what, void *base){	
	cfd = accept(fd, (struct sockaddr *)&client_addr, &addrlen);	
	if(cfd <= 0){
		perror("accept error\n");
		exit(1);
	}
	printf("accept: fd = %u\n", cfd);
	
	//使用bufferevent_socket_new创建一个struct bufferevent *bev，关联该sockfd，托管给event_base
	struct bufferevent *bev = bufferevent_socket_new((struct event_base *)base, cfd, BEV_OPT_CLOSE_ON_FREE);

	bufferevent_setcb(bev, read_cb, NULL, error_cb, base);
	bufferevent_enable(bev, EV_READ|EV_WRITE|EV_PERSIST);//启用read/write事件  
	
}

int main(){
	
	start_server();

	//创建基础事件
	base = event_base_new();
	
	if(base == NULL){
		perror("event_base_new error!\n");
		exit(1);
	}
	//struct event *event_new(struct event_base *, evutil_socket_t, short, event_callback_fn, void *);
    listener_event = event_new(base, listenfd, EV_READ|EV_PERSIST, do_accept, (void *)base);
	
	//if(event_base_set(base, listener_event) == -1){
		//perror("event_base_set error!\n");
		//exit(1);
	//}
	
	if(event_add(listener_event, NULL) == -1){
		perror("event_add error!\n");
		exit(1);
	}
	
	//运行事件循环
	if(event_base_dispatch(base) == -1){
		perror("event_base_dispatch error!\n");
		exit(1);
	}
		
	ret = signal(SIGINT, sig_handle);
	if(SIG_ERR == ret){
		printf("signal SIGSTOP occur erro\n");
		return -1;
	}
	
	event_base_free(base);
	return 0;
}
