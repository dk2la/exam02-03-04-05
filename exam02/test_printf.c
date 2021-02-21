#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>

int	g_return;
int	g_width;
int	g_dot;

static void	ft_putchar(int c)
{
	write(1, &c, 1);
	g_return++;
}

static void	ft_putstr(char *str, int len)
{
	while (*str && len--)
		{ft_putchar(*str); str++;}
}

static int	ft_strlen(char *str)
{
	int	len = 0;

	while (*str++)
		len++;
	return (len);
}

static char	*ft_itoa(long long nbr, int base)
{
	int			len = 0;
	char		*ret;
	long long	nbr_tmp = nbr;

	if (nbr == 0)
	{
		ret = malloc(sizeof(char) + 1);
		ret[0] = '0';
		ret[1] = '\0';
		return (ret);
	}
	if (nbr < 0)
		len++;
	while (nbr_tmp)
		{nbr_tmp /= base; len++;}
	if (!(ret = malloc(sizeof(char) * len + 1)))
		return (NULL);
	ret[len] = '\0';
	if (nbr < 0)
		{nbr *= -1; ret[0] = '-';}
	while (nbr)
	{
		ret[len - 1] = nbr % base + (nbr % base > 9 ? 87 : 48);
		nbr /= base;
		len--;
	}
	return (ret);
}

static void	ft_print_d(int nbr_tmp)
{
	char		*str;
	long long	nbr = nbr_tmp;
	int			len = 0;

	if (g_dot >= 0 && nbr == 0)
	{
		while (g_width > 0)
			{ft_putchar(' '); g_width--;}
		return ;
	}
	if (g_dot >= 0 && nbr < 0)
		{nbr *= -1; g_width--;}
	str = ft_itoa(nbr, 10);
	len = ft_strlen(str);
	if (g_dot >= 0 && len > g_dot)
		g_dot = len;
	if (g_dot >= 0)
		while (g_width - g_dot > 0)
			{ft_putchar(' '); g_width--;}
	else
		while (g_width - len > 0)
			{ft_putchar(' '); g_width--;}
	if (g_dot >= 0 && nbr_tmp < 0)
		ft_putchar('-');
	if (g_dot >= 0)
		while (g_dot - len > 0)
			{ft_putchar('0'); g_dot--;}
	ft_putstr(str, len);
	free(str);
}

static void	ft_print_x(unsigned int nbr)
{
	char	*str;
	int		len = 0;

	if (g_dot >= 0 && nbr == 0)
	{
		while (g_width > 0)
			{ft_putchar(' '); g_width--;}
		return ;
	}
	str = ft_itoa(nbr, 16);
	len = ft_strlen(str);
	if (g_dot >= 0 && len > g_dot)
		g_dot = len;
	if (g_dot >= 0)
		while (g_width - g_dot > 0)
			{ft_putchar(' '); g_width--;}
	else
		while (g_width - len > 0)
			{ft_putchar(' '); g_width--;}
	if (g_dot >= 0)
		while (g_dot - len > 0)
			{ft_putchar('0'); g_dot--;}
	ft_putstr(str, len);
	free(str);
}

static void	ft_print_s(char *str)
{
	int	len;

    if (!str)
        str = "(null)";
    len = ft_strlen(str);
	if (g_dot >= 0 && len < g_dot)
		g_dot = len;
	if (g_dot >= 0)
		while (g_width - g_dot > 0)
			{ft_putchar(' '); g_width--;}
	else
		while (g_width - len > 0)
			{ft_putchar(' '); g_width--;}
	if (g_dot >= 0)
	{
		while (g_dot - len > 0)
			{ft_putchar(' '); g_dot--;}
		ft_putstr(str, g_dot);
	}
	else
		ft_putstr(str, len);
}

static int	ft_dot(const char *str, int i)
{
	g_dot++;
	i++;
	while (str[i] >= '0' && str[i] <= '9')
		g_dot = g_dot * 10 + str[i++] - 48;
	return (i);
}

static int	ft_width(const char *str, int i)
{
	while (str[i] >= '0' && str[i] <= '9')
		g_width = g_width * 10 + str[i++] - 48;
	return (i);
}

int ft_printf(const char *str, ... )
{
	va_list	av;
	int		i = -1;

	g_return = 0;
	va_start(av, str);
	while (str[++i])
	{
		if (str[i] == '%' && str[i + 1])
		{
			g_width = 0;
			g_dot = -1;
			i++;
			if (str[i] >= '0' && str[i] <= '9')
				i = ft_width(str, i);
			if (str[i] == '.')
				i = ft_dot(str, i);
			if (str[i] == 's')
				ft_print_s(va_arg(av, char *));
			else if (str[i] == 'd')
				ft_print_d(va_arg(av, int));
			else if (str[i] == 'x')
				ft_print_x(va_arg(av, unsigned int));
		}
		else if (str[i] != '%')
			ft_putchar(str[i]);
	}
	va_end(av);
	return (g_return);
}