#include "common.h"
#include "stru_common.h"

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
		
		DATA data = {0};
		read(connfd, &data, sizeof(data));
		printf("read data:\n");
		printf("char     :%c\n", data.ch);
		printf("int      :%d\n", data.integer);
		printf("double   :%f\n", data.d);
		printf("str      :%s\n", data.str);
		
		close(connfd);
	}
	close(sockfd);
	return 0;
}
