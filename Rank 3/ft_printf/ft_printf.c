#include <unistd.h>
#include <stdarg.h>
#include <stdio.h>

void    ft_str(char *str, int *len)
{
    if(str == NULL)
        str = "(null)";
    while(*str)
        *len += write(1, str++, 1);
}

void    hex_digit(long long int num, int base, int *len)
{
    if(num < 0)
    {
        num *= -1;
        *len += write(1, "-", 1);
    }
    if(num >= base)
        hex_digit((num / base), base, len);
    *len += write(1, &"0123456789abcdef"[num % base], 1);
}

int ft_printf(const char *str, ...)
{
    int len;

    len = 0;
    va_list(pointer);
    va_start(pointer, str);
    while(*str)
    {
        if(*str == '%' && (*str + 1))
        {
            str++;
            if(*str == 's')
                ft_str(va_arg(pointer, char *), &len);
            if(*str == 'd')
                hex_digit((long long int)(va_arg(pointer, int)), 10, &len);
            if(*str == 'x')
                hex_digit((long long int)(va_arg(pointer, unsigned int)), 16, &len);
        }
        else
            len += write(1, str, 1);
        str++;
    }
    va_end(pointer);
    return (len);
}
int main(void)
{
    int num = 42;
    char *str = "ola mundio";
    unsigned int hex = 0xAABBCCDD;

    printf("printf: \n");
    printf("num: %d\nstr: %s\nhex: %x\n", num, str, hex);
    printf("ft_printf: \n");
    ft_printf("num: %d\nstr: %s\nhex: %x\n", num, str, hex);
    return (0);
}

