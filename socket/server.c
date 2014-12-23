#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>

#define PORT 8888
#define BACKLOG 2


int main(int argc, char *argv[])
{
    int ss, sc;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    int err;
    pid_t pid;

    void process_conn_server(int);
    
    ss = socket(AF_INET, SOCK_STREAM, 0);
    if(ss < 0)
        {
		printf("socket error!\n");
		return -1;
        }
	

	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(PORT);
	
	err = bind(ss, (struct sockaddr*)&server_addr, sizeof(server_addr));
	if(err < 0)
	{
		printf("bind error!\n");
		return -1;
	}


	err = listen(ss, BACKLOG);
        
        if(err < 0)
	{
		printf("listen error!\n");
		return -1;
        }


	for(;;){
            socklen_t addrlen = sizeof(client_addr);
            sc = accept(ss, (struct sockaddr*)&client_addr, &addrlen);
            if(sc < 0){
                continue;
            }
            pid = fork();
            
            if(pid == 0){
                
                process_conn_server(sc);
                close(ss);
            }else{
                close(sc);
            }

	}


    return 0;
}

void process_conn_server(int s)
{
    ssize_t size = 0;
    char buffer[1024];
    
    while(1){
        size = read(s, buffer, 1024);
        if(size == 0){
            return;
        }
        
        //sprintf(buffer, "%d bytes receive", size);
        
        write(s, buffer, strlen(buffer));
    }
    
}
