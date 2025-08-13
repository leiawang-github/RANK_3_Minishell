#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 42
#endif 

char	  *get_next_line(int fd);

#endif

char *ft_strdup(char *dest)
{
    int i;
    char *src;
    i = 0;
    while (dest[i])
        i++;
    src = (char *)malloc(sizeof(char) * i + 1);
    if (!src)
        return (NULL);
    i = 0;
    while (dest[i])
    {
        src[i] = dest[i];
        i++;
    }
    src[i] = '\0';
    return (src);
}
char	  *get_next_line(int fd)
{
    static char buffer[BUFFER_SIZE];
    static int buffer_read;
    static int buffer_pos;
    char line[70000];
    int i;
    i = 0;
    if (fd < 0 || BUFFER_SIZE <= 0)
        return (NULL);
    while (1)
    {
        if (buffer_pos >= buffer_read)
        {
            buffer_read = read(fd, buffer, BUFFER_SIZE);
            buffer_pos = 0;
            if (buffer_read <= 0)
                break;
        }
        line[i++] = buffer[buffer_pos++];
        if (buffer[buffer_pos] == '\n')
            break;
    }
    line[i] = '\0';
    if (i == 0)
        return (NULL);
    return (ft_strdup(line));
}