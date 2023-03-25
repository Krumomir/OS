#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <err.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

char **tokenize(char *line)
{
    char **tokens = malloc(sizeof(char *));

    if (tokens == NULL)
    {
        return NULL;
    }

    char *token;
    int i = 0;

    while(token = strtok(line, " "))
    {
        tokens[i++] = token;
        tokens = realloc(tokens, sizeof(char *) * (i + 1));
        line = NULL;

        if(tokens == NULL)
        {
            return NULL;
        }
    }
    tokens[i] = NULL;

    return tokens;
}


int main(int argc, char **argv)
{
	int commandStatus;
	int pipes[2][2];
	char **commands = (char **)malloc(4 * sizeof(char *));
	int commandCount = 4;
	
	commands[0] = strdup("cut -d : -f 7 /etc/passwd");
	commands[1] = strdup("sort");
	commands[2] = strdup("uniq");
	commands[3] = strdup("wc -l");
	
	pipe(pipes[0]);
	pipe(pipes[1]);	
	
	for(int i = 0; i < commandCount; i++)
	{
		int pid = fork();
		
		if(0 == pid)
		{
			char **command = tokenize(commands[i]);
			
			printf("%s\n", command[0]);
			
			if(0 == i)
				dup2(pipes[i % 2][1], 1);			
			else if(3 == i)
				dup2(pipes[abs((i % 2) - 1)][0], 0);
			else
			{
				dup2(pipes[i % 2][1], 1);
				dup2(pipes[abs((i % 2) - 1)][0], 0);		
			}
			
			close(pipes[1][0]);
			close(pipes[1][1]);
			
			close(pipes[0][0]);
			close(pipes[0][1]);
				
			execvp(command[0], command);
		}
		else
		{
			if(0 == i)
			{
				wait(&commandStatus);
				close(pipes[0][1]);
			}
			else if(1 == i)
			{
				wait(&commandStatus);
				close(pipes[1][1]);
				close(pipes[0][0]);
				
				pipe(pipes[0]);
			}
			else if(2 == i)
			{
				wait(&commandStatus);
				close(pipes[1][0]);
				close(pipes[0][1]);
			}
			else
			{
				wait(&commandStatus);
				close(pipes[0][0]);
			
				for(int j = 0; j < 4; j++)
					free(commands[j]);
				
				free(commands);
			}
		}		
	}
	
	return 0;
}
