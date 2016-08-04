#include "common.h"

#define PORT 8888

int main(int argc, char *argv[])
{
	int ret;
	int sockfd;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
		err_sys("socket error");

	struct sockaddr_in addr;
	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	ret = bind(sockfd, (struct sockaddr*)&addr, sizeof(addr));
	if(ret < 0)
		err_sys("bind error");

	ret = listen(sockfd, 5);
	if(ret < 0)
		err_sys("listen error");

	while(1)
	{
		struct sockaddr_in cli_addr;
		bzero(&cli_addr, sizeof(cli_addr));
		socklen_t addrlen = sizeof(cli_addr);
		int connfd;
		char cli_ip[32] = {0};

		connfd = accept(sockfd, (struct sockaddr*)&cli_addr, &addrlen);
		printf("client ip:%s, port:%d\n",
				inet_ntop(AF_INET, &cli_addr.sin_addr, cli_ip, sizeof(cli_ip)),
				ntohs(cli_addr.sin_port));
		
		FILE *fp;
		fp = fopen("recv_file.data", "w+");
		if(fp == NULL)
			err_sys("fopen error");
		
		fd_set fds;
		FD_ZERO(&fds);
		char buffer[1024];
		int n;
		while(1)
		{
			FD_SET(connfd, &fds);
			select(connfd + 1, &fds, NULL, NULL, NULL);
			if(FD_ISSET(connfd, &fds))
			{
				n = read(connfd, buffer, 1024);	
				if(n == 0)
				{
					printf("client closed\n");
					break;
				}
				else if(n > 0)
				{
					fwrite(buffer, 1, n, fp);
				}
				else
					err_sys("read error");
			}
		}
		close(connfd);
		fclose(fp);
	}
	close(sockfd);
	return 0;
}
