#include <unistd.h>
#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>

int putstr(char *str)
{
    int i = 0;
    int count = 0;
    if(!str)
        return 0;

    while(str[i] != '\0')
    {
        write(1, &str[i], 1);
        i++;
        count++;
    }
    return count; // count 表示打印出几个digits
}

int putnbr(int n)
{
    int count = 0;
    char c;

    if (n == -2147483648)
    {
        write(1, "-2147483648", 11);
        return 12;
    }
    else if(n < 0)
    {
        putstr("-");
        count++;
        n = -n;
    }
    if(n > 9)
        count+= putnbr(n / 10);
    c = n % 10 + '0';
    write(1, &c, 1);  
    count++;

    return count;
}

int puthex(unsigned int n)
{
    int count =0;
    char *hex = "0123456789abcdef";
    if (n >= 16)
        count+= puthex(n / 16);
    write(1, &hex[n % 16], 1);
    return (count+1);
}


int ft_printf(const char *str, ... )
{
    va_list args;
    int count = 0;
    int i = 0;
    va_start(args, str);

    while (str[i] != '\0')
    {
        if (str[i] == '%')
        {
            i++;
            if (str[i] == 's')
                count+= putstr(va_arg(args, char *));
            else if (str[i] == 'd')
                count+= putnbr(va_arg(args, int));
            else if (str[i] == 'x')
                count+= puthex(va_arg(args, unsigned int));
            else
            {
                write(1, &str[i], 1);
                count++;
            }
        }
        else
        {
            write(1, &str[i], 1);
            count++;
        }
        i++;
    }
    va_end(args);
    return (count);

}

int main()
{
    int res = ft_printf("123456"); 
    printf("\n");
    printf("%d\n",res);

}