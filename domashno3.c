#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <err.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void dup69(int fd1, int fd2)
{
	int res = dup2(fd1, fd2);
	
	if(res == -1)
		errx(3, "dup2 failed");
}

int main(int argc, char **argv)
{
	int commandStatus;
	int pipes[2][2];
	int commandCount = 4;
	
	char* cut[] = {"cut", "-f", "7", "-d", ":", "/etc/passwd", NULL};
    char* sort[] = {"sort", NULL};
    char* uniq[] = {"uniq", NULL};
    char* wc[] = {"wc", "-l", NULL};
    char** commands[] = {cut, sort, uniq, wc};

	if(pipe(pipes[0]) == -1)
		errx(1, "pipe failed");
		
	if(pipe(pipes[1]) == -1)
		errx(1, "pipe failed");
		
	for(int i = 0; i < commandCount; i++)
	{
		int pid = fork();
		
		if(-1 == pid)
			errx(2, "fork failed");
		
		if(0 == pid)
		{
		    (i == 0) ? 
		    	dup69(pipes[i % 2][1], 1) : 
		        (i == 3) ? 
		        	dup69(pipes[abs((i % 2) - 1)][0], 0) : 
		        	(dup69(pipes[i % 2][1], 1), dup69(pipes[abs((i % 2) - 1)][0], 0));

		    close(pipes[0][0]);
		    close(pipes[0][1]);

		    close(pipes[1][0]);
		    close(pipes[1][1]);

		    if(execvp(commands[i][0], commands[i]) == -1)
		    	errx(4, "execvp failed");
		}
		else
		{
			if(wait(&commandStatus) == -1)
            {

				close(pipes[0][0]);
				close(pipes[0][1]);

				close(pipes[1][0]);
				close(pipes[1][1]);				
				
                err(7, "wait failed");
            }
            
           	if(!WIFEXITED(commandStatus))
            {
                close(pipes[0][0]);
				close(pipes[0][1]);

				close(pipes[1][0]);
				close(pipes[1][1]);		
				
				errx(8, "child failed to do anything");
            }
			 
			 if(1 == i)
				if(pipe(pipes[0]) == -1)
					errx(1, "pipe failed");
				
			 close(pipes[i % 2][1]);	
		}	
	}
	
	return 0;
}
