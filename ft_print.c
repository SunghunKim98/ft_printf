# include "ft_print.h"



void	fake_ft_printf(const char *args, ...){
	va_list ap;

	va_start(ap, args);
	while (*args) {
		if (*args == '%') {
			args++;
			if (*args == 'd' || *args == 'i') {
				int num = va_arg(ap, int);
				ft_putnbr_fd(num, 1);
			}
			else if (*args == 's') {
				char *str = va_arg(ap, char*);
				ft_putstr_fd(str, 1);
			}
		} else {
			ft_putchar_fd(*args, 1);
		}
		args++;
	}
}

void	init_info(f_info *info)
{
	info->flag_minus = 0;
	info->flag_zero = 0;
	info->precision = -1;
	info->precision_star = 0;
	info->type = 0;
	info->width = 0;
	info->width_star = 0;
}

void	one_compartment_parse(char **args, f_info *info)
{
	init_info(info);
	while (!ft_strchr("cspdiuxX%", **args)) {
		if (info->width == 0 && info->precision == -1 && **args == '0')
			info->flag_zero = 1;
		else if (info->precision == -1 && ft_isdigit(**args))
			info->width = 10 * info->width + **args - '0';
		else if (info->precision >= 0 && ft_isdigit(**args))
			info->precision = 10 * info->precision + **args - '0';
		else if (**args == '-')
			info->flag_minus = 1;
		else if (**args == '.')
			info->precision = 0;
		else if (**args == '*' && info->precision == -1)
			info->width_star = 1;
		else if (**args == '*' && info->precision >= 0)
			info->precision_star = 1;
		(*args)++;
	}
	if (info->precision >= 0 && info->flag_zero == 1) // presion이 있으면 zero flag는 무시한다.
		info->flag_zero = 0;
	info->type = **args;

	// printf("***\n");
	// printf("flag_minus: %d\n", info->flag_minus);
	// printf("flag_zero: %d\n", info->flag_zero);
	// printf("precision: %d\n", info->precision);
	// printf("precision_star: %d\n", info->precision_star);
	// printf("type: %c\n", info->type);
	// printf("width: %d\n", info->width);
	// printf("width_star: %d\n", info->width_star);
	// printf("***\n");
}

int		find_digit(int num)
{
	int length;

	length = 0;
	if (num < 0)
	{
		length++;
		num = -num;
	}
	while (num > 0)
	{
		length++;
		num = num / 10;
	}
	return (length);
}

void	deal_stars(f_info *info, va_list ap)
{
	if (info->width_star)
	{
		info->width = va_arg(ap, int);
		if (info->width < 0) // width가 음수면, - flag를 켠다.
		{
			info->width = -(info->width);
			info->flag_minus = 1;
		}
	}
	if (info->precision_star)
	{
		info->precision = va_arg(ap, int);
		if (info->precision < 0) // precision이 0이면 어떻게 하지??? -> 그냥 없는 걸루.
			info->precision = -1;
	}
	// 여기서 무시되는 것들을 처리해야 할 듯. -> -가 있으면 0은 무시된다.
	if (info->flag_minus)
		info->flag_zero = 0;
}

void	set_other(char *other, char ch, int size)
{
	int i;

	i = 0;
	while(i < size)
		(other)[i++] = ch;
	(other)[i] = '\0';
}

char	*attach_minus(char *buff, int m)
{
	char	*minus;
	char	*total;

	if (m > 0)
	{
		if(!(minus = (char*)malloc(sizeof(char) * 2)))
			return (-1);
		minus[0] = '-';
		minus[1] = '\0';
		total = ft_strjoin(minus, buff);
		free(minus);
		free(buff);
	}
	else
		total = buff;
	return (total);
}

int		combine_and_print(int other_size, char *buff, f_info *info, int minus)
{
	char	*other;
	char	*print_stuff;
	int		return_size;
	char	*p;

	if (!(other = (char*)malloc(sizeof(char) * (other_size + 1))))
		return (-1);
	set_other(other, ' ', other_size);
	if (info->flag_minus) // zero flag, minus_flag를 각각 처리하자.
		print_stuff = ft_strjoin(buff, other);
	else if (info->flag_zero)
	{
		set_other(other, '0', other_size);
		print_stuff = ft_strjoin(other, buff);
	}
	else
		print_stuff = ft_strjoin(other, buff);
	p = attach_minus(print_stuff, minus);
	return_size = ft_putstr(p);
	free(other);
	free(p);
	free(buff);
	return (return_size);
}

char	*combine(int num, f_info *info) // number와 length만큼 0 또는 ' '를 붙이는 함수
{
	int		len;
	int		set;
	char	*str1;
	char	*str2;
	char	*str;

	len = find_digit(num);
	set = info->precision - len;
	str1 = (char*)malloc(sizeof(char) * (set + 1));
	set_other(str1, '0', set);
	str2 = ft_itoa(num);
	str = ft_strjoin(str1, str2);
	free(str1);
	free(str2);
	return (str);
}

int		combine_double(char *str, f_info *info)
{
	int len;
	char *str1;
	char *total;

	len = info->width - info->precision;
	str1 = (char*)malloc(sizeof(char) * (len + 1));
	set_other(str1, ' ', len);
	if (info->flag_minus)
		total = ft_strjoin(str, str1);
	else if (info->flag_zero)
	{
		set_other(str1, '0', len);
		total = ft_strjoin(str1, str);
	}
	else
		total = ft_strjoin(str1, str);
	len = ft_putstr(total);
	free(str1);
	free(str); // -> 이걸 여기서? -> ㅇㅇ
	free(total);
	return (len);
}

int		process_1_1(int num, f_info *info)
{
	char	*str;
	int		length;

	str = combine(num, info);
	if (info->width > info->precision)
		return (combine_double(str, info)); // 출력된 수가 return됨.
	length = ft_putstr(str);
	free(str);
	return (length);
}

int		process_1_2(int num, f_info *info)
{
	int		len;
	char	*buff;

	len = find_digit(num);
	if (!(buff = ft_itoa(num)))
		return (-1);
	if (info->width > len)
		return (combine_and_print(info->width - len, buff, info, 0));
	else
	{
		ft_putstr(buff);
		free(buff);
		return (len);
	}
}

char	*add_minus(char *str1, char *str2)
{
	char *str;

	str = ft_strjoin(str1, str2);
	free(str1);
	free(str2);
	return (str);
}

int		print_str(char *str)
{
	int num;
	
	num = ft_putstr(str);
	free(str);
	return (num);
}

int		process_2_1(int num, f_info *info)
{
	char	*buff;
	int		len;

	len = find_digit(num);
	if (info->flag_zero)
	{
		if (info->width >= len)
			return (print_str(ft_strjoin_minus(info->width - len, ft_itoa(-num))));
		else
			return (print_and_return(num));
	}
	buff = ft_strjoin_minus(info->precision - len + 1, ft_itoa(-num));
	len = ft_strlen(buff);
	if (info->width > ft_strlen(buff))
	{
		if (info->flag_minus)
			return (print_str(ft_strjoin_free(buff, mk_space_with(' ', info->width - len))));
		else
			return (print_str(ft_strjoin_free(mk_space_with(' ', info->width - len), buff)));
	}
	else
		return (print_str(buff));
}

int		process_2_2(int num, f_info *info)
{
	char	*buff;
	int		len;

	buff = ft_itoa(num);
	len = ft_strlen(buff);
	if (info->width > len)
	{
		if (info->flag_minus)
			return (print_str(ft_strjoin_free(buff, mk_space_with(' ', info->width - len))));
		else
			return (print_str(ft_strjoin_free(mk_space_with(' ', info->width - len), buff)));
	}
	else
		return (print_str(buff));
}

int		print_d(f_info *info, va_list ap)
{
	int len;
	int	num;

	deal_stars(info, ap); // 첫 번째로, *을 먼저 처리하자. 이것들을 인자로 받아오자.
	num = va_arg(ap, int);
	if (num >= 0)
	{
		len = find_digit(num); // 숫자의 크기 123이면 3
		if (info->precision > len)
			return (process_1_1(num, info));
		if (info->precision == -1 || info->precision <= len) // precision이 없을 때 또는 의미가 없을 때.
			return (process_1_2(num, info));
	}
	else
	{
		len = find_digit(num);
		if (info->precision >= len || info->flag_zero == 1)
			return (process_2_1(num, info)); // precision이 있을 때.
		if (info->precision == -1 || info->precision < len)
			return (process_2_2(num, info));
	}
}

int		divide_each_type(f_info *info, va_list ap)
{
	if (info->type == 'c')
	{

	}
	else if (info->type == 's')
	{

	}
	else if (info->type == 'p')
	{

	}
	else if (info->type == 'd')
		print_d(info, ap);
	else if (info->type == 'i')
	{

	}
	else if (info->type == 'u')
	{

	}
	else if (info->type == 'x' || info->type == 'X')
	{

	}
}

int		print_stuff(char *args, va_list ap)
{
	f_info	*info;
	int		total;

	if (!(info = (f_info*)malloc(sizeof(f_info))))
		return (-1);
	total = 0;
	while (*args) {
		if (*args == '%'){
			args++;
			one_compartment_parse(&args, info); // In here, I parsed all attributes in format_info
			divide_each_type(info, ap);
		}
		else {
			ft_putchar_fd(*args, 1); // 여기도 좀 살펴;
			total++;
		}
		args++;
	}
	free(info);
	return (total);
}

int		ft_printf(const char *args, ...)
{
	va_list ap;
	int		total;

	total = 0;
	va_start(ap, args);

	total = print_stuff((char*)args, ap);

	return (total);
}

// %[flags][width][precision][type]



int main()
{

	// precision이 있을 때.

	
	ft_printf("|%.3d|\n", -4242);  // |4242|
    ft_printf("|%1.1d|\n", -4242); // |4242|
    ft_printf("|%6.3d|\n", -4242); // |  4242|
    ft_printf("|%3.6d|\n", -4242); // |004242|
    ft_printf("|%.7d|\n", -4242);  // |0004242|
    ft_printf("|%4.7d|\n", -4242); // |0004242|
    ft_printf("|%10.7d|\n", -4242);// |   0004242|

    ft_printf("|%.3d|\n", -4242);  // |4242|
    ft_printf("|%.4d|\n", -4242);  // |4242|
    ft_printf("|%.5d|\n", -4242);  // |4242|

	printf("-------\n");

	// precision이 있을 때.


	printf("|%.3d|\n", -4242);  // |4242|
    printf("|%1.1d|\n", -4242); // |4242|
    printf("|%6.3d|\n", -4242); // |  4242|
    printf("|%3.6d|\n", -4242); // |004242|
    printf("|%.7d|\n", -4242);  // |0004242|
    printf("|%4.7d|\n", -4242); // |0004242|
    printf("|%10.7d|\n", -4242);// |   0004242|

    printf("|%.3d|\n", -4242);  // |4242|
    printf("|%.4d|\n", -4242);  // |4242|
    printf("|%.5d|\n", -4242);  // |4242|



	return 0;
}
