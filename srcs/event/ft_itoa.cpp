#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char	*ft_allocspace(int digits, int *n)
{
	char	*result;

	if (*n < 0)
	{
		result = (char *)malloc(sizeof(char) * (digits + 2));
		result[0] = '-';
		*n = -*n;
	}
	else
		result = (char *)malloc(sizeof(char) * (digits + 1));
	if (!result)
		return (NULL);
	return (result);
}

static int	ft_countdigits(long long int n)
{
	long int		digits;

	digits = 0;
	while (n != 0)
	{
		n /= 10;
		digits++;
	}
	return (digits);
}

static char	*assign(long int n1, int digits, int count, char *result)
{
	int		i;

	i = 0;
	if (n1 < 0)
	{
		i++;
		digits++;
		n1 *= -1;
	}
	while (i < digits)
	{
		result[digits - count] = ((n1 % 10) + '0');
		n1 /= 10;
		i++;
		count++;
	}
	result[i] = '\0';
	return (result);
}

char	*ft_itoa(int n)
{
	int			digits;
	char		*result;
	int			count;
	long long	n1;

	n1 = n;
	digits = ft_countdigits(n);
	count = 1;
	if (n1 == 0)
	{
		result = (char *)malloc(2);
		result[0] = '0';
		result[1] = '\0';
		return (result);
	}
	else
		result = ft_allocspace(digits, &n);
	if (!result)
		return (NULL);
	assign(n1, digits, count, result);
	return (result);
}
