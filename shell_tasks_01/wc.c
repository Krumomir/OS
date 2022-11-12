#include<stdio.h>
#include <fcntl.h>
#include<stdlib.h>
#include <unistd.h>
#include <string.h>
#include <err.h>

int getLines(int fd)
{
    int numLines = lseek(fd, 0, SEEK_END);

    if(-1 == lseek(fd, 0, SEEK_SET) || numLines == -1)
    {
        close(fd);
        errx(EXIT_FAILURE, "lseek failed");
    }

    return numLines;
}

int getNewLines(int fd)
{
    char buffer;
    int result;
    int counterNewLine = 0;

    while(((result = read(fd, &buffer, 1)) != -1) && result != 0)
        if(buffer == '\n')
            counterNewLine++;

    if(result == -1)
    {
        close(fd);
        errx(EXIT_FAILURE, "read failed");
    }

    return counterNewLine;
}

int main(int argc, char **argv)
{
    int fd = open(argv[1], O_RDONLY);

    if(fd == -1)
        errx(EXIT_FAILURE, "Can`t open file");

    if(argc == 2)
        printf("%d %d\n", getNewLines(fd), getLines(fd));
    else if(!strcmp(argv[2], "-l"))
    {
        printf("%d", getNewLines(fd));

        if(argc >= 4 && !strcmp(argv[3], "-c"))
            printf(" %d\n", getLines(fd));

    }
    else if(!strcmp(argv[2], "-c"))
    {
        printf("%d", getLines(fd));

        if(argc >= 4 && !strcmp(argv[3], "-l"))
            printf(" %d\n", getNewLines(fd));
    }

    close(fd);

    return 0;
}
