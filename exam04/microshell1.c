#include <stdlib.h>
#include <unistd.h>
#include <string.h>

typedef struct	s_fd
{
	int		write;
	int		read;
	int		flag;
}				t_fd;

int		ft_strlen_d(char **av)
{
	int		i;

	i = 0;
	while (av[i])
		i++;
	return (i);
}

void	ft_putstr(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		write(2, &str[i], 1);
		i++;
	}
}

void	exit_fatal()
{
	ft_putstr("HELLO");
	exit(EXIT_FAILURE);
}

void	get_fd(t_fd *fd)
{
	int		fds[2];

	if ((pipe(fds)) == -1)
		exit_fatal();
	fd->write = fds[1];
	fd->read = fds[0];
	fd->flag = 1;
}

void	cd(char **av)
{
	if (ft_strlen_d(av) != 2)
		ft_putstr("wrong number arguments\n");
	else if ((chdir(av[1])) == -1)
	{
		ft_putstr("HELLO");
		ft_putstr(av[0]);
		ft_putstr("\n");
	}
}

void	start_process(char **av, t_fd *fd, char **envp)
{
	pid_t	pid;

	if (!strcmp("cd", av[0]))
		cd(av);
	if ((pid = fork()) == -1)
		exit_fatal();
	else if (pid == 0)
	{
		if ((execve(av[0], av, envp)) == -1)
		{
			ft_putstr("HELLO");
			ft_putstr(av[0]);
			ft_putstr("\n");
		}
	}
	else
		waitpid(pid, 0, 0);
}

void	fork_process(char **av, t_fd *fd, char **envp)
{
	pid_t pid;

	if ((pid = fork()) == -1)
		exit_fatal();
	else if (pid == 0)
	{
		dup2(fd->write, 1);
		close(fd->read);
		start_process(av, fd, envp);
		close(fd->write);
		exit(1);
	}
	else
	{
		dup2(fd->read, 0);
		close(fd->write);
		waitpid(pid, 0, 0);
		close(fd->read);
	}
}

void	exec_cmd(char **av , t_fd *fd, char **envp)
{
	if (fd->flag)
		fork_process(av, fd, envp);
	else
		start_process(av, fd, envp);
}

int		main(int ac, char **av, char **envp)
{
	t_fd	fd;
	int		i = 0;
	int		tmp_fd = dup(0);

	av++;
	(void)ac;
	while(av[i])
	{
		fd.flag = 0;
		if (!strcmp("|", av[i]))
		{
			av[i] = 0;
			if (i)
			{
				get_fd(&fd);
				exec_cmd(av, &fd, envp);
			}
			av += i + 1;
			i = 0;
		}
		if (!strcmp(";", av[i]))
		{
			av[i] = 0;
			if (i)
				exec_cmd(av, &fd, envp);
			av += i + 1;
			i = 0;
			dup2(tmp_fd, 0);
		}
		if (!av[i + 1])
		{
			exec_cmd(av, &fd, envp);
			av += i + 1;
			i = 0;
			dup2(tmp_fd, 0);
		}
		else
			++i;
	}
	close(tmp_fd);
	return (0);
}