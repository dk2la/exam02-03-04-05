#include <unistd.h>

int		main(int argc, char **argv)
{
	int i;
	int j;
	int k;
	int f;

	i = 0;
	f = 0;
	j = 0;
	if (argc == 3)
	{
		while (argv[1][i])
		{
			k = i - 1;
			f = 0;
			while (k >= 0)
			{
				if (argv[1][k] == argv[1][i])
				{
					f = 1;
					break;
				}
				k--;
			}
			if (!f)
			{
				while (argv[2][j])
				{
					if (argv[1][i] == argv[2][j])
					{
						write(1, &argv[1][i], 1);
						break ;
					}
					j++;
				}
			}
			i++;
		}
	}
	write(1, "\n", 1);
	return (0);
}