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
	struct s_cli* next;
}				t_cli;

int		g_id;
char	buffer[300000];
fd_set	read_fd, write_fd, cp_read_fd, cp_write_fd;

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

void	fatal_error(int sock) {
	write(2, "Fatal error\n", strlen("Fatal error\n"));
	close(sock);
	exit(1);
}

void	send_all(t_cli* tmp, char* msgg, int fd) {
	for (t_cli* it = tmp; it; it = it->next) {
		if (FD_ISSET(it->fd, &cp_write_fd) && it->fd != fd) {
			send(it->fd, msgg, strlen(msgg), 0);
		}
	}
}

void	rm_rf(t_cli **temp, int id) {
	t_cli* tmp = *temp;
	t_cli* del;

	if (tmp->id == id) {
		*temp = tmp->next;
		free(tmp);
	}
	else {
		while (tmp->next && tmp->next->id != id)
			tmp = tmp->next;
		del = tmp->next;
		tmp = tmp->next->next;
		free(del);
	}
}

int		main(int ac, char** av) {
	if (ac != 2) {
		write(2, "Wrong number of arguments\n", strlen("Wrong number of arguments\n"));
		exit(1);
	}
	int		max_fd, ret, clinet_fd, sock_fd;
	struct sockaddr_in server_addr, cli_addr;
	socklen_t add_len = sizeof(cli_addr);
	char	*buff = (char*)calloc(300000, 1);
	t_cli	*temp;
	char*	chunk = NULL;
	uint16_t port = atoi(av[1]);

	bzero(&temp, sizeof(temp));
	bzero(&server_addr, sizeof(server_addr));

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = 127 | (1 << 24); //127.0.0.1
	server_addr.sin_port = port >> 8 | port << 8;

	if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		fatal_error(sock_fd);
	if ((bind(sock_fd, (const struct sockaddr *)&server_addr, sizeof(server_addr))) != 0)
		fatal_error(sock_fd);
	if (listen(sock_fd, 10) != 0)
		fatal_error(sock_fd);

	max_fd = sock_fd;
	FD_ZERO(&read_fd);
	FD_ZERO(&write_fd);
	FD_SET(sock_fd, &read_fd);
	FD_SET(sock_fd, &write_fd);
	bzero(&buffer, sizeof(buffer));
	while (1)
	{
		cp_write_fd = write_fd;
		cp_read_fd = read_fd;
		select(max_fd + 1, &cp_read_fd, &cp_write_fd, 0, 0);
		if (FD_ISSET(sock_fd, &cp_read_fd))
		{
			if ((clinet_fd = accept(sock_fd, (struct sockaddr*)&cli_addr, &add_len)) > 0)
			{
				FD_SET(clinet_fd, &read_fd);
				FD_SET(clinet_fd, &write_fd);
				max_fd = (clinet_fd > max_fd) ? clinet_fd : max_fd;
				t_cli* it = temp;
				t_cli* new;
				new = (t_cli*)malloc(1 * sizeof(t_cli));
				new->id = g_id++;
				new->fd	= clinet_fd;
				new->next = NULL;
				new->write_buff = NULL;
				new->read_buff = NULL;
				if (!temp)
					temp = new;
				else {
					while (it->next)
						it = it->next;
					it->next = new;
				}
				sprintf(buffer, "server: client %d just arrived\n", new->id);
				for(it = temp; it; it = it->next) {
					if (it->fd != clinet_fd)
						it->write_buff = str_join(it->write_buff, buffer);
				}
				bzero(&buffer, 200);
			}
		}
		for (t_cli* it = temp; it; it = it->next)
		{
			if (FD_ISSET(it->fd, &cp_read_fd))
			{
				printf("%s\n", "HERE");
				ret = recv(it->fd, buff, 299999, 0);
				if (ret <= 0)
				{
					printf("%s\n", "HERE1");
					sprintf(buffer, "server: client %d just left\n", it->id);
					for(t_cli* ite = temp; ite; ite = ite->next) {
						if (ite->fd != it->fd)
							ite->write_buff = str_join(ite->write_buff, buffer);
					}
					close(it->fd);
					FD_CLR(it->fd, &read_fd);
					FD_CLR(it->fd, &write_fd);
					if (it->read_buff)
						free(it->read_buff);
					if (it->write_buff)
						free(it->write_buff);
					rm_rf(&temp, it->id);
				}
				else if (ret > 0)
				{
					printf("%s\n", "HERE3");
					it->read_buff = str_join(it->read_buff, buff);
					while (extract_message(&(it->read_buff), &chunk)) {
						for (t_cli* ite = temp; ite; ite = ite->next) {
							if (it->fd != ite->fd) {
								ite->write_buff = str_join(ite->write_buff, chunk);
								printf("%s\n", "HERE4");
							}
						}
						free(chunk);
						chunk = NULL;
					}
					bzero(buff, ret);
				}
			}
		}
		for (t_cli* it = temp; it; it = it->next)
		{
			if (FD_ISSET(it->fd, &cp_write_fd) && it->write_buff)
			{
				ret = send(it->fd, it->write_buff, strlen(it->write_buff), 0);
				if (ret == -1) {
					sprintf(buffer, "server: client %d just left\n", it->id);
					for(it = temp; it; it = it->next) {
						if (it->id != clinet_fd)
							it->write_buff = str_join(it->write_buff, buffer);
					}
					close(it->fd);
					FD_CLR(it->fd, &read_fd);
					FD_CLR(it->fd, &write_fd);
					if (it->read_buff)
						free(it->read_buff);
					if (it->write_buff)
						free(it->write_buff);
					rm_rf(&temp, it->id);
				}
				else if (ret != strlen(it->write_buff)) {
					chunk = str_join(chunk, it->write_buff + ret);
					free(it->write_buff);
					it->write_buff = chunk;
					bzero(&chunk, sizeof(chunk));
				}
				else {
					free(it->write_buff);
					it->write_buff = NULL;
				}
			}
		}
	}
}
