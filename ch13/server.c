#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>

void sigchld_handle(int sig)
{
	while(waitpid(-1, 0, WNOHANG) > 0)
	;
	return;
}
void echo(int fd);
int main()
{
	int serverfd;
	struct sockaddr_in addr;
	/* 1. open socket */
	serverfd = socket(AF_INET, SOCK_STREAM, 0);
	if(-1 == serverfd){
		printf("open socket error\n");
		exit(-1);
	}

	/* 2.bind to socket */
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8888);
	inet_aton("192.168.153.135", &addr.sin_addr);
	if(-1 == bind(serverfd, (struct sockaddr *)&addr, sizeof(addr))){
		printf("bind socket error\n");
		close(serverfd);
		exit(-1);
	}

	/* 3.listen to socket*/
	if(-1 == listen(serverfd, 10)){
		printf("listen error\n");
		close(serverfd);
		exit(-1);
	}	

	signal(SIGCHLD, sigchld_handle);
	while(1){
		/* 4.accept client connect */
		struct sockaddr_in from_addr;
		int len = sizeof(from_addr);
		int fd = accept(serverfd, (struct sockaddr*)&from_addr, &len);
		printf("connect from:%s\n", inet_ntoa(from_addr.sin_addr));
		
		if(fork() == 0){
			/* 5. recv from clinet*/
			echo(fd);
			/* 6.1 close serverfd*/
			close(serverfd);
	   	/* 6.close clinet socket*/
	   	close(fd);
			exit(0);
		}	
	}

	/* 7. server socket */
	close(serverfd);

	return 0;
}

void echo(int fd)
{
	int buf;
	int ret;
	while(1){
		ret = recv(fd, &buf, MSG_WAITALL, 0);
		if(0 == ret){
			printf("The client is shutdown\n");
			break;
		}
		else if(-1 == ret){
			printf("netork error\n");
			break;
		}
		else{
			printf("read from clinet:%d\n", buf);
		}
	}
}
