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

#define PORT 8888
#define MAXCLINE 5 //连接队列中的个数
#define BUF_SIZE 200
#define BACKLOG 10
#define FD_MAX_SIZE 3
#define MAX_EVENTS 3
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
	  
	if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {  
        perror("setsockopt error");  
        exit(1);  
    } 
    
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

	printf("server is up...\n");
}

void do_read(evutil_socket_t cfd, short what, void *arg){	
	printf("do read\n");
	
	if((revlen = recv(cfd, buffer, sizeof(buffer), 0)) <= 0){
		
		//free event
		event_del((struct event *)arg);
		event_free((struct event *)arg);
				
		close(cfd);
		printf("close fd\n");
	}else{
		memset(buffer + revlen, '\0', 1);
		printf("receive data is: %s\n", buffer);
	}
}

void do_accept(evutil_socket_t fd, short what, void *base){	
	cfd = accept(fd, (struct sockaddr *)&client_addr, &addrlen);	
	if(cfd <= 0){
		perror("accept error\n");
		exit(1);
	}
	evutil_make_socket_nonblocking(cfd);
	struct event *ev = calloc(1, sizeof(struct event));
	ev = event_new((struct event_base *)base, cfd, EV_READ|EV_PERSIST, do_read, (void *)ev);
	
	if(event_add(ev, NULL) == -1){
		perror("event_add error!\n");
		exit(1);
	}
	
}

int main(){
	
	start_server();
	
	//把socket设置为非阻塞方式  
	evutil_make_socket_nonblocking(listenfd);

	//创建基础事件
	base = event_base_new();
	
	if(base == NULL){
		perror("event_base_new error!\n");
		exit(1);
	}
	//struct event *event_new(struct event_base *, evutil_socket_t, short, event_callback_fn, void *);
    listener_event = event_new(base, listenfd, EV_READ|EV_PERSIST, do_accept, (void *)base);
	
	if(event_base_set(base, listener_event) == -1){
		perror("event_base_set error!\n");
		exit(1);
	}
	
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
