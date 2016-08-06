#include "common.h"
#include "stru_common.h"

#define PORT 8888

int main(int argc, char *argv[])
{
	if(argc != 3)
		err_quit("Usage: ./file_client serv_name filename");
	int ret;
	int sockfd;
	struct sockaddr_in serv_addr;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
		err_sys("socket error");

	bzero(&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

    struct hostent *serv;
    serv = gethostbyname(argv[1]);
    if(serv == NULL)
        err_sys("gethostbyname error");
    char servip[32] = {0};
    inet_ntop(serv->h_addrtype, *(serv->h_addr_list), servip, sizeof(servip));
    printf("server ip:%s\n", servip);
    serv_addr.sin_addr.s_addr = *(in_addr_t*)(serv->h_addr_list[0]);
	//ret = inet_pton(AF_INET, argv[1], &serv_addr.sin_addr);
    //if(ret != 1)
      //  err_quit("invalid ip address");

	ret = connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	if(ret != 0)
		err_sys("connect fail");

	FILE *fp;
	fp = fopen(argv[2], "r");
	if(fp == NULL)
		err_sys("fopen error");

	char buffer[2048];
	int n;

	fd_set fds;
	FD_ZERO(&fds);
	while(1)
	{
		FD_SET(sockfd, &fds);
		select(sockfd + 1, NULL, &fds, NULL, NULL);
		if(FD_ISSET(sockfd, &fds))
		{
			n = fread(buffer, 1, sizeof(buffer), fp);
			if(n == 0)
				break;
			else
				write(sockfd, buffer, n);
		}
	}
	
	close(sockfd);
	return 0;
}
