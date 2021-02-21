#include <unsistd.h>

int     main(int argc, char **argv)
{
    int i;
    int j;
    int k;
    int f;
    int n;

    i = 0;
    j = 0;
    f = 0;
    if (argc == 3)
    {
        while(argv[1][i])
        {
            k = i - 1;
            f = 0;
            while (k >= 0)
            {
                if (argv[1][k] == argv[1][i])
                {
                    f = 1;
                    break ;
                }
                k--;
            }
            if (!f)
                write (1, &argv[1][i], 1);
            i++;
        }
        while (argv[2][j])
        {
            n = i - 1;
            k = j - 1;
            f = 0;
            while (k >= 0)
            {
                if (argv[2][k] == argv[2][j])
                {
                    f = 1;
                    break;
                }
                k--;
            }
            while ()
        }
    }
}