# include "ft_print.h"

int		ft_putstr(char *s)
{
	int	size;

	if (s == 0)
		return (0);
	size = 0;
	while (*s)
	{
		write(1, s, 1);
		s++;
		size++;
	}
	return (size);
}

int		print_and_return(int num)
{
	ft_putnbr_fd(num, 1);
	return (find_digit(num));
}

char	*mk_space_with(char ch, int length)
{
	char	*str;
	int		i;

	str = (char*)malloc(sizeof(char) * (length + 1));
	i = 0;
	while (i < length)
		str[i++] = ch;
	str[i] = '\0';
	return (str);
}

char	*ft_strjoin_free(char *s1, char *s2)
{
	int		n1;
	int		n2;
	char	*p;
	int		i;

	if (s1 == 0 || s2 == 0)
		return (NULL);
	n1 = ft_strlen(s1);
	n2 = ft_strlen(s2);
	p = (char*)malloc(sizeof(char) * (n1 + n2 + 1));
	if (p == NULL)
		return (NULL);
	i = -1;
	while (++i < n1)
		p[i] = s1[i];
	i = -1;
	while (++i < n2)
		p[n1 + i] = s2[i];
	p[n1 + i] = '\0';
	free(s1);
	free(s2);
	return (p);
}


char	*ft_strjoin_minus(int length, char *str)
{
	char	*tmp;
	char	*buff;

	tmp = ft_strjoin_free(mk_space_with('0', length), str);
	buff = ft_strjoin_free(mk_space_with('-', 1), tmp);
	return (buff);
}