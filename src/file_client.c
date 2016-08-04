#include "common.h"
#include "stru_common.h"

#define PORT 8888
#define SERV_IP "127.0.0.1"

int main(int argc, char *argv[])
{
	if(argc != 2)
		err_quit("Usage: ./file_client filename");
	int ret;
	int sockfd;
	struct sockaddr_in serv_addr;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
		err_sys("socket error");

	bzero(&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	inet_pton(AF_INET, SERV_IP, &serv_addr.sin_addr);

	ret = connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	if(ret != 0)
		err_sys("connect fail");

	FILE *fp;
	fp = fopen(argv[1], "r");
	if(fp == NULL)
		err_sys("fopen error");

	char buffer[512];
	int n;

	fd_set fds;
	FD_ZERO(&fds);
	while(1)
	{
		FD_SET(sockfd, &fds);
		select(sockfd + 1, NULL, &fds, NULL, NULL);
		if(FD_ISSET(sockfd, &fds))
		{
			n = fread(buffer, 1, 512, fp);
			if(n == 0)
				break;
			else
				write(sockfd, buffer, n);
		}
	}
	
	close(sockfd);
	return 0;
}
