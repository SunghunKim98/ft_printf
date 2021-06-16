#ifndef FT_PRINT_H
# define FT_PRINT_H

# include <stdarg.h>
# include <unistd.h>
# include <stdio.h>

# include "./libft_evaluation/libft.h"

typedef struct format_info {
	int		flag_minus;
	int		flag_zero;
	int		width;
	int		width_star;
	int		precision;
	int		precision_star;
	char	type;
} f_info;


void	init_info(f_info *info);
void	one_compartment_parse(char **args, f_info *info);
int		find_digit(int num);
void	deal_stars(f_info *info, va_list ap);
void	set_other(char *other, char ch, int size);
int		combine_and_print(int other_size, char *buff, f_info *info, int minus);
char	*combine(int num, f_info *info); // number와 length만큼 0 또는 ' '를 붙이는 함수
int		combine_double(char *str, f_info *info);
int		process_1_1(int num, f_info *info);
int		process_1_2(int num, f_info *info);
char	*add_minus(char *str1, char *str2);
int		process_2_1(int num, f_info *info);
int		process_2_2(int num, f_info *info);
int		print_d(f_info *info, va_list ap);
int		divide_each_type(f_info *info, va_list ap);
int		print_stuff(char *args, va_list ap);
char	*attach_minus(char *buff, int m);

int		ft_printf(const char *args, ...);


int		ft_putstr(char *s);
int		print_and_return(int num);
char	*mk_space_with(char ch, int length);
char	*ft_strjoin_free(char *s1, char *s2);
char	*ft_strjoin_minus(int length, char *str);

#endif
