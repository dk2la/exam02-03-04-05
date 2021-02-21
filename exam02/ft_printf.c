#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

int		g_dot;
int		g_width;
int		g_return;

void	ft_printf_x(unsigned int num)
{
	char	*str;
	int		len;

	if (g_dot >= 0 && nbr == 0)
	{
		while (g_width > 0)
			{ft_puthcar(' '); g_width--;}
		return ;
	}
	if (g_dot >= 0 && nbr < 0)
	 {g_width -= 1; nbr *= -1;}
	str = ft_itoa(nbr, 10);
	len = ft_strlen(str);
	if (g_dot >= 0 && g_dot < len)
		g_dot = len;
	if (g_dot >= 0)
		while (g_width - g_dot > 0)
			{ft_putchar('0'); g_width--;}
	else
		while (g_width - len > 0)
			{ft_putchar('0'); g_width--;}
	if (g_dot >= 0 && num <= 0)
		ft_putchar('-');
	if (g_dot >= 0)
		while (g_dot - len > 0)
			{ft_putchar('0'); g_dot--;}
	ft_putstr(str, g_dot);
}

void	ft_printf_d(int num)
{
	char		*str;
	long long	nbr = num;
	int			len = 0;

	if (g_dot >= 0 && nbr == 0)
	{
		while (g_width > 0)
			{ft_puthcar(' '); g_width--;}
		return ;
	}
	if (g_dot >= 0 && nbr < 0)
	 {g_width -= 1; nbr *= -1;}
	str = ft_itoa(nbr, 10);
	len = ft_strlen(str);
	if (g_dot >= 0 && g_dot < len)
		g_dot = len;
	if (g_dot >= 0)
		while (g_width - g_dot > 0)
			{ft_putchar(' '); g_width--;}
	else
		while (g_width - len > 0)
			{ft_putchar(' '); g_width--;}
	if (g_dot >= 0 && num <= 0)
		ft_putchar('-');
	if (g_dot >= 0)
		while (g_dot - len > 0)
			{ft_putchar('0'); g_dot--;}
	ft_putstr(str, g_dot);
}

int		ft_printf_s(char *str)
{
	int len = ft_strlen(str);

	if (g_dot >= 0 && g_dot > len)
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

int		ft_parse_dot(const char *format, int i)
{
	g_dot = 0;
	i++;
	while (format[i] >= '0' && format[i] <= '9')
		g_dot = g_dot * 10 + (format[i++] - 48);
	return (i);
}

int		ft_parse_width(const char *format, int i)
{
	while (format[i] >= '0' && format[i] <= '9')
		g_width = g_width * 10 + (format[i++] - 48);
	return (i);
}

int		ft_printf(const char *format, ...)
{
	int i;
	va_list av;

	i = -1;
	g_return = 0;
	va_start(av, format);
	while (format[++i])
	{
		if (format[i] == '%' && format[i + 1])
		{
			g_dot = -1;
			g_width = 0;
			i++;
			if (format[i] >= '0' && format[i] <= '9')
				i = ft_parse_width(format, i);
			if (format[i] == '.')
				i = ft_parse_dot(format, i);
			if (format[i] == 's')
				i = ft_printf_s(va_arg(av, char *));
			if (format[i] == 'd')
				i = ft_printf_d(va_arg(av, int));
			if (format[i] == 'x')
				i = ft_printf_x(va_arg(av, unsigned int));
		}
		else if (format[i] != '%')
			ft_putchar_fd(format[i], 0);
	}
}