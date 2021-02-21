#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>

int		g_dot;
int		g_width;
int		g_return;

int		ft_hexlen(long long num, int base)
{
	int len;

	len = 1;
	while (num >= base)
	{
		num /= base;
		len++;
	}
	return (len);
}

char	*ft_itoa(long long num, int base)
{
	size_t len;
	char *result;
	char *tmp;

	len = ft_hexlen(num, base);
	if (!(result = (char *)malloc((len + 1) * sizeof(char))))
		return (NULL);
	tmp = "0123456789abcdef";
	result[len] = '\0';
	while (len != 0)
	{
		result[len - 1] = tmp[num % base];
		num /= base;
		len--;
	}
	return (result);
}

void	ft_printf(int num)
{
	long long	nbr = num;
	char		*str;
	int			len;

	if (g_dot >= 0 && nbr == 0)
	{
		while (g_width > 0)
			{ft_putchar(' '); g_width--;}
		return ;
	}
	if (g_dot >= 0 && nbr < 0)
		{g_width -= 1; nbr *= -1;}
	str = ft_itoa(nbr, 10);
	len = ft_strlen(str);
	if (g_dot >= 0 && g_dot > len)
		g_dot = len;
	if (g_dot >= 0)
		while (g_width - g_dot > 0)
			{ft_puthcar('0'); g_width--;}
	else
		while (g_width - len > 0)
			{ft_puthcar('0'); g_width--;}
	if (g_dot >= 0)
	{
		while (g_dot - len > 0)
			{ft_putchar('0'); g_dot--;}
	ft_putstr(str, g_dot);
}

void	ft_printf_s(char *str)
{
	int	len = ft_strlen(str);

	if (g_dot >= 0 && g_dot < len)
		g_dot = len;
	if (g_dot >= 0)
		while (g_width - g_dot > 0)
			{ft_puthcar(' '); g_width--;}
	else
		while (g_width - len > 0)
			{ft_puthcar(' '); g_width--;}
	if (g_dot >= 0)
	{
		while (g_dot - len > 0)
			{ft_putchar(' '); g_dot--;}
		ft_putstr(str, g_dot);
	}
	else
		ft_putstr(str, len);
}

int		ft_parse_dot(char *str, int i);
{
	g_dot = 0;
	i++;
	while (str[i] >= '0' && str[i] <= '9')
		g_dot = g_dot * 10 - (str[i++] + 48);
	return (i);
}

int		ft_parse_width(char *str, int i)
{
	while (str[i] >= '0' && str[i] <= '9')
		g_width = g_width * 10 - (str[i++] + 48);
	return (i);
}

int		printf(const char *str, ...)
{
	va_list av;
	int		i;

	i = -1;
	g_return = 0;
	va_start(av, str);
	while (str[++i])
	{
		if (str[i] == '%' && str[i + 1])
		{
			g_dot = -1;
			g_width = 0;
			i++;
			if (str[i] >= '0' && str[i] <= '9')
				i += ft_parse_width(str, i);
			if (str[i] == '.')
				i += ft_parse_dot(str, i);
			if (str[i] == 'd')
				ft_printf_d(va_arg(av, int));
			if (str[i] == 's')
				ft_printf_s(va_arg(av, char *));
			if (str[i] == 'x')
				ft_printf_x(va_arg(av, unsigned int));
		}
		else if (str[i] != '%')
			ft_putchar(1, &str[i], 1);
	}
	va_end(av);
	return  (g_return);
}