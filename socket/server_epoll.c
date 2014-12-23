#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include <sys/epoll.h>
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


void setnonblocking(int sock)  
{  
    int opts = fcntl(sock,F_GETFL);  
    if(opts<0)  
    {  
        perror("fcntl(sock,GETFL)");  
        exit(1);  
    }  
    opts = opts|O_NONBLOCK;  
    if(fcntl(sock,F_SETFL,opts)<0)  
    {  
        perror("fcntl(sock,SETFL,opts)");  
        exit(1);  
    }  
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

int main(){
	
	start_server();
	
	//把socket设置为非阻塞方式  
	setnonblocking(listenfd);
	
	//声明epoll_event结构体的变量,ev用于注册事件,数组用于回传要处理的事件  
    struct epoll_event ev, events[20]; 
    
    //创建一个epoll的句柄，size用来告诉内核这个监听的数目一共有多大  
	epfd = epoll_create(FD_MAX_SIZE);
	
	//设置与要处理的事件相关的文件描述符  
    ev.data.fd=listenfd;  
      
    //设置要处理的事件类型  
    ev.events=EPOLLIN|EPOLLET;  
  
    //注册epoll事件  
    epoll_ctl(epfd,EPOLL_CTL_ADD,listenfd,&ev);  
    
    int nfds = 0;
    while(1){
		// int epoll_wait(int epfd, struct epoll_event * events, int maxevents, int timeout);
		nfds = epoll_wait(epfd, events, MAX_EVENTS, 600000);
		
		if(nfds <= 0){
			perror("time is out!\n");
			continue;
		}
		
		int i;
		for(i = 0; i < nfds; i++){
			if(events[i].data.fd == listenfd){
				cfd = accept(listenfd, (struct sockaddr *)&client_addr, &addrlen);

				if(cfd <= 0){
					perror("accept error\n");
					exit(1);
				}
				ev.data.fd = cfd;
				ev.events = EPOLLIN|EPOLLET;
				epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &ev);

			}else if(events[i].events & EPOLLIN){
				printf("event is EPOLLIN\n");
				if((cfd = events[i].data.fd) < 0){
					continue;
				}
				
				if((revlen = recv(cfd, buffer, sizeof(buffer), 0)) < 0){
					if (errno == ECONNRESET){  
                        close(cfd);
                        events[i].data.fd = -1;  
					}else
						printf("receive error\n");
				}else if (revlen == 0) //读入的数据为空  
                {  
                    close(cfd);  
                    events[i].data.fd = -1;  
                }
                
                memset(buffer + revlen, '\0', 1);
                printf("receive data is: %s\n", buffer);
                
                 //设置用于写操作的文件描述符  
                ev.data.fd = cfd;  
                  
                //设置用于注册的写操作事件  
                ev.events=EPOLLOUT|EPOLLET;  
                  
                //修改sockfd上要处理的事件为EPOLLOUT  
				epoll_ctl(epfd, EPOLL_CTL_MOD, cfd, &ev); /* 修改 */  
			}else if(events[i].events & EPOLLOUT){
				printf("event is EPOLLOUT\n");
				if((cfd = events[i].data.fd) < 0){
					continue;
				}
				
				char msg[] = "great!\n";
				send(cfd, msg, sizeof(msg), 0);
				
				//设置用于读操作的文件描述符  
                ev.data.fd=cfd;  
                 
                //设置用于注册的读操作事件  
                ev.events=EPOLLIN|EPOLLET;   
                  
                //修改sockfd上要处理的事件为EPOLIN  
                epoll_ctl(epfd,EPOLL_CTL_MOD,cfd,&ev); /* 修改 */  
			}		
			
		}		
		

		ret = signal(SIGINT, sig_handle);
		if(SIG_ERR == ret){
			printf("signal SIGSTOP occur erro\n");
			return -1;
		}
	}
	
	close(epfd);
	return 0;
}
