#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include <sys/select.h>
#include <sys/time.h>
#include <errno.h>  

#define PORT 8888
#define MAXCLINE 5 //连接队列中的个数
#define BUF_SIZE 200
#define BACKLOG 10 


int sfd, cfd;
struct sockaddr_in server_addr;
struct sockaddr_in client_addr;
int err;
int yes;
    
void start_server(){
	sfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sfd < 0)
    {
		perror("socket error!\n");
		exit(1);
    }
	  
	if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {  
        perror("setsockopt error");  
        exit(1);  
    } 
    
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(PORT);


	err = bind(sfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
	if(err < 0)
	{
		perror("bind error");  
        exit(1); 
	}

	err = listen(sfd, BACKLOG);      

    if(err < 0)
	{
		perror("listen error");  
        exit(1);  
    }


	printf("server is up...\n");
}

int main(int argc, char *argv[])
{
	start_server();	
	
	fd_set rfds;
	struct timeval tv;
	
	int conn_num = 0;
	int maxfd = 0;
	
	int fd[MAXCLINE] = {0};
	
	char buf[BUF_SIZE];
	
	tv.tv_sec = 600;
	tv.tv_usec = 0;
	
	maxfd = sfd;
	
	//fd[conn_num] = sfd;
	
	while(1){
		FD_ZERO(&rfds);
		FD_SET(sfd, &rfds);
		
		int i;
		for(i = 0; i < MAXCLINE; i++){
			if(fd[i] != 0){			
				FD_SET(fd[i], &rfds);
			}
		}
		
		int ret = select(maxfd + 1, &rfds, NULL, NULL, &tv);
		
		if(ret < 0){
			perror("select occur error");
			exit(1);
		}
		
		if(ret == 0){
			perror("time is out");
			exit(1);
		}
		
		int j;
		for(j = 0; j < conn_num; j++){
			if(FD_ISSET(fd[j], &rfds)){
				ret = recv(fd[j],buf,sizeof(buf),0);
				char str[] = "Good,very nice!\n";
				
				send(fd[j], str, sizeof(str), 0);
				
				if(ret <= 0){
					printf("client %d is closed\n", fd[j]);
					close(fd[j]);
					FD_CLR(fd[j], &rfds);
					fd[j] = 0;
					conn_num--;
				}else{
					if(ret < BUF_SIZE){
						memset(&buf[ret], '\0', 1);
						printf("client[%d] send:%s\n",j,buf);
						
					}
				}	
			}				
		}
		
		if(FD_ISSET(sfd, &rfds)){
			socklen_t addrlen = sizeof(struct sockaddr_in);
			int cfd = cfd = accept(sfd, (struct sockaddr *)&client_addr, &addrlen);
						
			if(cfd <= 0){
				perror("accept error\n");
				continue;
			}
			
			if(conn_num < MAXCLINE){
				int m;
				for(m = 0; m < MAXCLINE; m++){			
					if(fd[m] == 0){			
						fd[conn_num++] = cfd;
						printf("new a client cfd is:%d\n", cfd);
						if(cfd > maxfd){
						maxfd = cfd;
						}
						break;
					}
				}	
			}else{
				char send_msg[] = "the number of client over the max";
				send(cfd, send_msg, sizeof(send_msg), 0);
				close(cfd);
				
			}
		}	
	}
	
	int k;
		for(k = 0; k < MAXCLINE; k++){
			if(fd[k] != 0){
				close(fd[k]);
			}			
		}
}
	
