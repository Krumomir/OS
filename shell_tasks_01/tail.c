#include<stdio.h>
#include <fcntl.h>
#include<stdlib.h>
#include <unistd.h>
#include <string.h>
#include <err.h>

int getNewLines(int fd)
{
    char buffer;
    int result;
    int counterNewLine = 0;

    while(((result = read(fd, &buffer, 1)) != -1) && result != 0)
        if(buffer == '\n')
            counterNewLine++;

    if(lseek(fd, -1, SEEK_CUR) == -1)
    {
        close(fd);
        errx(EXIT_FAILURE, "lseek failed");
    }
    else if(result == -1)
    {
        close(fd);
        errx(EXIT_FAILURE, "read failed");
    }

    return counterNewLine;
}

int getNLine(int fd, int numLines)
{
    char buffer;
    int result;
    int counterNewLine = 0;

    if(numLines <= 0)
    {
        if(lseek(fd, 0, SEEK_SET) == -1)
        {
            close(fd);
            errx(EXIT_FAILURE, "lseek failed");
        }

        return 0;
    }

    while(((result = read(fd, &buffer, 1)) != -1) && result != 0 && counterNewLine < numLines)
        if(buffer == '\n')
            counterNewLine++;

    if(lseek(fd, -1, SEEK_CUR) == -1)
    {
        close(fd);
        errx(EXIT_FAILURE, "lseek failed");
    }
    else if(result == -1)
    {
        close(fd);
        errx(EXIT_FAILURE, "read failed");
    }



    return counterNewLine;
}

void printLastNLines(int fd, int numLines)
{
    int result;
    char buffer;

    getNLine(fd, getNewLines(fd) - numLines);

    while(((result = read(fd, &buffer, 1)) != -1) && result != 0)
        printf("%c", buffer);

    if(result == -1)
    {
        close(fd);
        errx(EXIT_FAILURE, "read failed");
    }
}

int main(int argc, char **argv)
{
    int fd = open(argv[1], O_RDONLY);
    int numLines = 0;

    if(fd == -1)
        errx(EXIT_FAILURE, "Can`t open file");

    if(argc == 2 || !argv[3])
        printLastNLines(fd, 10);
    else if(!strcmp(argv[2], "-n"))
        numLines = atoi(argv[3]);

    if(numLines > 0)
        printLastNLines(fd, numLines);

    close(fd);

    return 0;
}
