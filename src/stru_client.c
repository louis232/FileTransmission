#include "common.h"
#include "stru_common.h"

#define PORT 8888
#define SERV_IP "127.0.0.1"

int main(int argc, char *argv[])
{
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

	DATA data = {'c', 12345, 3.14, "hello"};
	write(sockfd, &data, sizeof(data));
	
	close(sockfd);
	return 0;
}
