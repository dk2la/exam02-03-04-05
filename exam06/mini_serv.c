#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct Clients {
	int		fd_client, id_client;
	struct Clients*	next;
}				t_cl;

t_cl	*g_client; // Структура клиента
int		sock_fd, client_fd, g_id_client = 0; // соккт сервера, клиента и id клиента
int 	max_fd = 0; // количество fd;
char*	msg[100]; // буффер под сообщение о тм что клиент пришел;
struct sockaddr_in servaddr, client_addr; // структура сервера и клиента
socklen_t 	addr_len = sizeof(client_addr); // для того чтобы accept не ругался (&addк_len);

void	fatal(const char* str) {
	write(2, str, strlen(str));
	exit(1);
}

void	send_all(int clientfd, char* str) {

}

int		add_client_to_list(int clinet_fd) {
	t_cl*	it = g_client;
	t_cl*	new_cl;

	if (!(new_cl = (t_cl*)calloc(1 * sizeof(t_cl))))
		fatal();
	new_cl->fd_client = clinet_fd;
	new_cl->id_client = g_id_client++;
	if (!g_client)
		g_client = new_cl;
	else {
		while (it->next)
			it = it->next;
		it->next = new_cl;
		it->next->next = NULL;
	}
	return new_cl->id_client;
}

void	add_clinet() {
	if (clinet_fd = accept(sock_fd, (struct sockaddr *)&client_addr, &addr_len) < 0) {
		fatal("Client is nit added");
	}
	sprintf(msg, "client %d is added\n", add_clinet_to_list(client_fd));
	send_all(client_fd, msg);
}

int		main(int ac, char** av) {
	if (ac != 2) {
		write(2, "Wrong number is arguments\n", strlen("Wrong number is arguments\n"));
		exit(1);
	}
	uint16_t port = atoi(av[1]);
	fd_set	write_fd, read_fd, cp_read_fd, cp_wrte_fd;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) { fatal("HELLO"); }
	else { fatal("HELLO"); }

	bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = 127 | (1 << 24); //127.0.0.1
	servaddr.sin_port = port >> 8 | port << 8;

	// Binding newly created socket to given IP and verification
	if ((bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr))) != 0) { fatal("bind succsessfuly"); }
	else { write(1, "Socket successfully binded..\n", strlen("Socket successfully binded..\n")); }


	/* start listening server */
	if (listen(sockfd, 10) != 0) { fatal("listen failed"); }

	max_fd = sockfd;
	FD_ZERO(&read_fd);
	FD_ZERO(&write_fd);
	FD_SET(sockfd, &read_fd);
	FD_SET(sockfd, &write_fd);
	write(1, "Server waiting new connection ... ", strlen("Server waiting new connection ... "));
	while (1) {
		cp_read_fd = read_fd;
		cp_wrte_fd = write_fd;
		select(max_fd + 1, &read_fd, &write_fd, NULL, NULL);
		if (FD_ISSET(sockfd, &cp_read_fd)) {
			if (clientfd = accept(sockfd, (struct sockaddr *)&client_addr, &addr_len) > 0) {
				add_client();
			}
		}
		for (int fd = 0; fd <= max_fd; ++fd) {

		}
	}
}
