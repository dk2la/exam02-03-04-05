#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

char	*func(char *str, char c, int *size)
{
	char	*res;
	int		i;

	i = 0;
	res = (char *)malloc((*size + 1) * sizeof(char));
	while (str && i < (*size))
	{
		res[i] = str[i];
		i++;
	}
	free(str);
	str = NULL;
	res[*size] = c;
	*size += 1;
	return (res); 
}

int		get_next_line(char **line)
{
	char	buffer[1];
	int		size;
	int		bytes;
	char	*tmp;

	tmp = NULL;
	size = 0;
	while ((bytes = read(0, buffer, 1)) > 0)
	{
		if (buffer[0] == '\n')
		{
			if (!(tmp = func(tmp, '\0', &size)))
				return (-1);
			*line = tmp;
			return (1);
		}
		if (!(tmp = func(tmp, buffer[0], &size)))
			return (-1);
	}
	if (bytes == -1)
	{
		if (tmp)
			free(tmp);
		return (-1);
	}
	else if (bytes == 0)
	{
		if (!tmp)
		{
			*line = malloc(1);
			*line[0] = '\0';
			return (0);
		}
		else
		{
			tmp = func(tmp, '\0', &size);
			*line = tmp;
			return (0);
		}
	}
	return (-1);
}

int		main()
{
	char	*s;

	s = NULL;
	while (get_next_line(&s))
		printf("%s\n", s);
	return (0);
}