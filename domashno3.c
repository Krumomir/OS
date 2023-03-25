#include <stdio.h>
#include "/home/krumomir/OS/Terminal/main.c"
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <err.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char **argv)
{
	char *str = strdup("cut -d : -f 7 /etc/passwd");
	char *str1 = strdup("sort");
	char *str2 = strdup("uniq");
	char *str3 = strdup("wc");
	int status;
	int pipe1 [2];
	int pipe2 [2];
	
	pipe(pipe1);
	pipe(pipe2);
	
	int pidCut = fork();
	
	if(pidCut == 0)
	{
		char **command = tokenize(str);
	
		dup2(pipe1[1], 1);
	
		close(pipe1[0]);
		close(pipe1[1]);			
		
		execvp(command[0], command);
	}
	
	wait(&status);
	close(pipe1[1]);
	
	int pidSort = fork();
	
	if(pidSort == 0)
	{
		dup2(pipe1[0], 0);
		dup2(pipe2[1], 1);
		
		close(pipe1[0]);
		close(pipe1[1]);
		
		close(pipe2[0]);
		close(pipe2[1]);	
		
		execlp(str1, str1, NULL);
	}
		
	wait(&status);
	close(pipe2[1]);
	close(pipe1[0]);	
	pipe(pipe1);
	
	int pidUniq = fork();

	if(pidUniq == 0)
	{
		dup2(pipe2[0], 0);
		dup2(pipe1[1], 1);
		
		close(pipe2[0]);
		close(pipe2[1]);
			
		close(pipe1[0]);
		close(pipe1[1]);	
		
		execlp(str2, str2, NULL);
	}
		
	wait(&status);
	close(pipe2[0]);
	close(pipe1[1]);
	
	int pidWc = fork();
	
	if(pidWc == 0)
	{
		dup2(pipe1[0], 0);
		
		close(pipe1[0]);
		close(pipe1[1]);	
		
		execlp(str3, str3, "-l", NULL);
	}
		
	wait(&status);
	close(pipe1[0]);
	
	free(str);
	free(str1);
	free(str2);
	free(str3);
	
	return 0;
}
