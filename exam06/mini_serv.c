#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/select.h>

typedef struct s_cli {
	char*	read_buff;
	char*	write_buff;
	int 	fd;
	int		id;
	int		active;
	struct s_cli* next;
}				t_cli;

int	g_id = 0;

int extract_message(char **buf, char **msg)
{
	char	*newbuf;
	int	i;

	*msg = 0;
	if (*buf == 0)
		return (0);
	i = 0;
	while ((*buf)[i])
	{
		if ((*buf)[i] == '\n')
		{
			newbuf = calloc(1, sizeof(*newbuf) * (strlen(*buf + i + 1) + 1));
			if (newbuf == 0)
				return (-1);
			strcpy(newbuf, *buf + i + 1);
			*msg = *buf;
			(*msg)[i + 1] = 0;
			*buf = newbuf;
			return (1);
		}
		i++;
	}
	return (0);
}

char *str_join(char *buf, char *add)
{
	char	*newbuf;
	int		len;

	if (buf == 0)
		len = 0;
	else
		len = strlen(buf);
	newbuf = malloc(sizeof(*newbuf) * (len + strlen(add) + 1));
	if (newbuf == 0)
		return (0);
	newbuf[0] = 0;
	if (buf != 0)
		strcat(newbuf, buf);
	free(buf);
	strcat(newbuf, add);
	return (newbuf);
}

void	fatal_error() {
	write(2, "Fatal error\n", strlen("Fatal error\n"));
	exit(1);
}

int		main(int ac, char** av) {
	if (ac != 2) {
		write(2, "Wrong number of arguments\n", strlen("Wrong number of arguments\n"));
		exit(1);
	}

	int sockfd, connfd, max_fd, ret, id_tmp;
	struct sockaddr_in servaddr, cli; 
	socklen_t	len = sizeof(cli);
	fd_set	fd_read, fd_write, cp_fd_read, cp_fd_write;
	uint16_t port = atoi(av[1]);
	t_cli*	temp;
	char*	msg = NULL;
	char*	buff = NULL;
	char*	buff_one = (char*)calloc(101000, 1);

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		fatal_error(); 
	bzero(&servaddr, sizeof(servaddr)); 

	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = 127 | (1 << 24); //127.0.0.1
	servaddr.sin_port = port >> 8 | port << 8; 
  
	if ((bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr))) != 0)
		fatal_error();
	if (listen(sockfd, 10) != 0)
		fatal_error();

	max_fd = sockfd;
	
	FD_ZERO(&fd_read);
	FD_ZERO(&fd_write);

	FD_SET(max_fd, &fd_read);
	FD_SET(max_fd, &fd_write);

	while (1) {
		cp_fd_read = fd_read;
		cp_fd_write = fd_write;
		select(max_fd + 1, &cp_fd_read, &cp_fd_write, NULL, 0);
		if (FD_ISSET(sockfd, &cp_fd_read)) {
			connfd = accept(sockfd, (struct sockaddr *)&cli, &len);
			if (connfd) {
				t_cli* 	tmp = temp;
				t_cli*	new;

				if (!(new = (t_cli*)malloc(1 * sizeof(t_cli))))
					fatal_error();
				new->id = g_id++;
				new->active = 1;
				new->fd = connfd;
				new->write_buff = NULL;
				new->read_buff = NULL;
				new->next = NULL;
				if (!temp)
					temp = new;
				else {
					while (tmp->next)
						tmp = tmp->next;
					tmp->next = new;
				}
				FD_SET(connfd, &fd_write);
				FD_SET(connfd, &fd_read);
				sprintf(buff_one, "server: client %d just arrived\n", g_id - 1);
				for (tmp = temp; tmp; tmp = tmp->next) {
					if (tmp->active && tmp->fd != connfd)
						tmp->write_buff = str_join(tmp->write_buff, buff_one);
				}
				bzero(buff_one, 100);
				max_fd = (connfd > max_fd) ? connfd : max_fd;
			}
		}
		for (int i = 3; i <= max_fd; ++i) {
			if (FD_ISSET(i, &cp_fd_read) && i != sockfd) {
				buff = (char*)calloc(4097, 1);
				ret = recv(i, buff, 4096, 0);
				if (ret <= 0) {
					t_cli*	tmp;
					t_cli*	del;

					for (tmp = temp; tmp; tmp = tmp->next) {
						if (tmp->active && tmp->fd == i) {
							id_tmp = tmp->id;
							break;
						}
					}
					sprintf(buff_one, "server: client %d just left\n", id_tmp);
					for (tmp = temp; tmp; tmp = tmp->next) {
						if (tmp->active && tmp->fd != i)
							tmp->write_buff = str_join(tmp->write_buff, buff_one);
					}
					bzero(buff_one, 100);
					tmp = temp;
					if (tmp->fd == i) {
						temp = tmp->next;
						tmp->active = 0;
						close(tmp->fd);
						FD_CLR(tmp->fd, &fd_read);
						FD_CLR(tmp->fd, &fd_write);
						if (tmp->write_buff)
							free(tmp->write_buff);
						if (tmp->read_buff)
							free(tmp->read_buff);
						free(tmp);
					}
					else {
						while (tmp->next && tmp->next->fd != i)
							tmp = tmp->next;
						del = tmp->next;
						tmp->next = tmp->next->next;
						del->active = 0;
						close(del->fd);
						FD_CLR(del->fd, &fd_read);
						FD_CLR(del->fd, &fd_write);
						if (del->write_buff)
							free(del->write_buff);
						if (del->read_buff)
							free(del->read_buff);
						free(del);
					}
				}
				else if (ret > 0) {
					t_cli*	tmp = temp;
					t_cli*	cur;
					for (tmp = temp; tmp; tmp = tmp->next) {
						if (tmp->active && tmp->fd == i) {
							cur = tmp;
							break;
						}
					}
					cur->read_buff = str_join(cur->read_buff, buff);
					while (extract_message(&cur->read_buff, &msg)) {
						sprintf(buff_one, "client %d: %s", cur->id, msg);
						t_cli* tmp2 = temp;
						for (; tmp2; tmp2 = tmp2->next) {
							if (tmp2->active && tmp2->fd != i)
								tmp2->write_buff = str_join(tmp2->write_buff, buff_one);
						}
						bzero(buff_one, 101000);
						msg = NULL;
					}
					free(buff);
				}
			}
		}
		for (t_cli* tmp = temp; tmp; tmp = tmp->next) {
			if (FD_ISSET(tmp->fd, &cp_fd_write) && tmp->write_buff && tmp->fd != sockfd) {
				ret = send(tmp->fd, tmp->write_buff, strlen(tmp->write_buff), 0);
				if (ret == -1) {
					t_cli*	cur;
					t_cli*	del;
					sprintf(buff_one, "server: client %d just left\n", tmp->id);
					for (cur = temp; cur; cur = cur->next) {
						if (cur->active && cur->fd != tmp->fd)
							cur->write_buff = str_join(cur->write_buff, buff_one);
					}
					cur = temp;
					FD_CLR(tmp->fd, &fd_read);
					FD_CLR(tmp->fd, &fd_write);
					if (tmp->write_buff)
						free(tmp->write_buff);
					if (tmp->read_buff)
						free(tmp->read_buff);
					tmp->active = 0;
					if (cur->fd == tmp->fd) {
						temp = cur->next;
						free(cur);
					}
					else {
						while (cur->next && cur->next->fd != tmp->fd)
							cur = cur->next;
						del = cur->next;
						cur = cur->next->next;
						close(tmp->fd);
						free(del);
					}
					bzero(buff_one, 100);
				}
				else if (ret != strlen(tmp->write_buff)) {
					msg = str_join(msg, tmp->write_buff + ret);
					free(tmp->write_buff);
					tmp->write_buff = msg;
					free(msg);
					msg = NULL;
				}
				else {
					free(tmp->write_buff);
					tmp->write_buff = NULL;
				}
			}
		}
	}
	return (0);
}
