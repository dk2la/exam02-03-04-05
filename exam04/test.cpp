#include <iostream>

int     readNumber(int x)
{
    std::cin >> x;
    return (x);
}

void    writeAnswer(int x)
{
    std::cout << x << std::endl;
}

int     main()
{
    int a = 0;
    int b = 0;

    a = readNumber(a);
    b = readNumber(b);
    writeAnswer(a + b);
    return (0);
}