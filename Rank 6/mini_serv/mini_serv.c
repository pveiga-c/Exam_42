#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>

typedef struct client
{
	int id;
	char msg[150000];
} t_client;

t_client clients[2048];
fd_set write_fds, read_fds, active_fds;
char write_buff[424242], read_buff[424242];
int max_fds = 0, next_id = 0;

void error(char *msg)
{
	write(2, msg, strlen(msg));
	exit(1);
}

void sendMsg(int sender)
{
	for(int fd = 0; fd <= max_fds; fd++)
		if(FD_ISSET(fd, &write_fds) && fd != sender)
			write(fd, write_buff, strlen(write_buff));
}

int main(int argc, char **argv)
{
	if(argc != 2)
		error("Wrong number of arguments\n");

	int sockfd = max_fds = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
		error("Fatal error\n");

	FD_ZERO(&active_fds);
	FD_SET(sockfd, &active_fds);

	struct sockaddr_in servaddr;
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(2130706433);
	servaddr.sin_port = htons(atoi(argv[1]));
  
	if ((bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr))) != 0 || listen(sockfd, 10) != 0)
		error("Fatal error\n");
	
	while(1)
	{
		read_fds = write_fds = active_fds;
		if(select(max_fds + 1, &read_fds, &write_fds, NULL, NULL) < 0)
			continue;

		for(int fd = 0; fd <= max_fds; fd++)
		{
			if(FD_ISSET(fd, &read_fds) && fd == sockfd)
			{
				int client_fd = accept(sockfd, NULL, NULL);

				max_fds = (client_fd > max_fds) ? client_fd : max_fds;
				clients[client_fd].id = next_id++;
				bzero(clients[client_fd].msg, strlen(clients[client_fd].msg));

				FD_SET(client_fd, &active_fds);

				sprintf(write_buff, "server: client %d just arrived\n", clients[client_fd].id);
				sendMsg(client_fd);
				bzero(clients[client_fd].msg, strlen(clients[client_fd].msg));
				break;
			}
			if(FD_ISSET(fd, &read_fds) && fd != sockfd)
			{
				int ret = recv(fd, &read_buff, sizeof(read_buff), 0);

				if(ret <= 0)
				{
					sprintf(write_buff, "server: client %d just left\n", clients[fd].id);
					sendMsg(fd);
					FD_CLR(fd, &active_fds);
					close(fd);
					break;
				}
				else
				{
					for(int i = 0, j = strlen(clients[fd].msg); i < ret; i++, j++)
					{
						clients[fd].msg[j] = read_buff[i];
						if(clients[fd].msg[j] == '\n')
						{
							clients[fd].msg[j] = '\0';
							sprintf(write_buff, "client %d: %s\n", clients[fd].id, clients[fd].msg);
							sendMsg(fd);
							bzero(clients[fd].msg, strlen(clients[fd].msg));
							j=-1;
						}
					}
					break;
				}
			}
		}
	}
}
