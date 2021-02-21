#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

int		width, height;
char	*draw;
char	type, color, back;
float	x, y, rad;

int		check_hit(float i, float j)
{
	float	dist;

	dist = sqrtf((i - x) * (i - x) + (j - y) * (j - y));
	if (dist <= rad)
	{
		if (dist - rad <= -1)
			return (1);
		return (2);
	}
	return (0);
}

void	drawing(void)
{
	int	i = -1, j, hit;
	while (++i < width)
	{
		j = -1;
		while (++j < height)
		{
			hit = check_hit((float)i, (float)j);
			if (hit == 2 || (hit == 1 && type == 'C'))
				draw[i + j * width] = color;
		}
	}
}

int		parser(FILE *file)
{
	int		i = 0;
	int		result;

	if(fscanf(file, "%d %d %c\n", &width, &height, &back) != 3)
		return (1);
	if (width <= 0 || width > 300 || height <= 0 || height > 300)
		return (1);
	if (!(draw = malloc(width * height)))
		return (1);
	while (i < width * height)
		draw[i++] = back;
	while ((result = fscanf(file, "%c %f %f %f %c\n", &type, &x, &y, &rad, &color)) == 5)
	{
		if (rad <= 0 || (type != 'c' && type != 'C'))
		{
		//	printf("%f\n", rad);
			return (1);
		}
		drawing();
	}
	if (result != -1)
		return (0);
	//printf("HERE\n");
	return (0);
}
void	output(void)
{
	int		i = -1, c = 10;

	while (++i < height)
	{
		write (1, draw + i * width, width);
		write (1, &c, 1);
	}
}

int		main(int ac, char **av)
{
	FILE *file;

	if (ac != 2)
	{
		write(1, "Error: arguments!\n", strlen("Error: arguments!"));
		return (1);
	}
	if (!(file = fopen(av[1], "r")) || parser(file))
	{
		write(1, "Error: Operation file corrupted!/n", strlen("Error: Operation file corrupted!"));
		return (1);
	}
	output();
	fclose(file);
	free(draw);
	return (0);
}