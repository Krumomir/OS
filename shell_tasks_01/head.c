#include<stdio.h>
#include<err.h>
#include <fcntl.h>
#include<stdlib.h>
#include <unistd.h>
#include <string.h>

void printNLines(int fd, int numLines)
{
    char buffer;
    int result;
    int counterNewLine = 0;

    while(((result = read(fd, &buffer, 1)) != -1) && result != 0 && counterNewLine < numLines)
    {
        if(buffer == '\n')
            counterNewLine++;

        printf("%c", buffer);
    }

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
        printNLines(fd, 10);
    else if(!strcmp(argv[2], "-n") && argv[3])
        numLines = atoi(argv[3]);

    if(numLines > 0)
        printNLines(fd, numLines);
        
    close(fd);

    return 0;
}
