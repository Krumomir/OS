#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


size_t strlen(char *);
char *strcat(char *, char *);
void swap(int *, int *);
void swap1(void *, void *, size_t);
char *strdup(char *);
char *readline(int);


int main()
{
	char *a = (char *)malloc(4 * sizeof(char));
	a[0] = 'f';
	a[1] = 'g';
	a[2] = 'c';
	a[3] = 'd';
	a[4] = 'e';
	char *b = (char *)malloc(3 * sizeof(char));
	b[0] = 'a';
	b[1] = 'b';
	
	int c = 1;
	int d = 2332;
	swap(&c, &d);	
	swap1(a, b, 2);
	char *e = strdup(a); 
	//printf("%s", e);
	
	int fd = open("pesho.txt", O_RDONLY);
	printf("%s", readline(fd));
	//ne obrushtai wnimanie kak si testwam neshtata
	//nadqwam se prowerkite da sa nalice tozi put :)	

	return 0;
}

size_t strlen(char *a)
{
	int temp = 0;
	while(a[temp] != '\0')
		temp++;
		
	return temp;
}

char *strcat(char *a, char *b)
{
	int lena = strlen(a);
	int lenb = strlen(b);

	char *result = (char *)malloc((lena + lenb + 1) * sizeof(char));
	
	if(!result)
		exit(2);
	
	for(int i = 0; i < lena; i++)
		result[i] = a[i];
		
	for(int i = 0; i < lenb; i++)
		result[lena + i] = b[i];
		
	return result;
}

void swap(int *a, int *b)
{
	int temp = *a;	
	*a = *b;
	*b = temp;
}

void swap1(void *a, void *b, size_t count)
{
	for(int i = 0; i < count; i++)
	{
		char temp = ((char *)a)[i];
		((char *)a)[i] = ((char *)b)[i];
		((char *)b)[i] = temp;
	}
}

char *strdup(char *a)
{
	char *result = (char *)malloc(1 * sizeof(char));
	
	if(!result)
		exit(1);
		
	*result = '\0';
	
	return strcat(result, a);
}

char *readline(int fd)
{
	int lenres = 1;
	char b;
	int a;
	
	if(lseek(fd, 0, SEEK_SET) == -1)
		exit(3);
	
	while(a = read(fd, &b, 1) && a != -1)
	{
		if(b == '\n')
			break;
			
		lenres++;
	}
	
	if(a == -1)
		exit(4);
	
	if(lseek(fd, 0, SEEK_SET) == -1)
		exit(5);
		
	char *result = (char *)malloc(lenres * sizeof(char));
	
	if(!result)
		exit(6);
		
	if(read(fd, result, lenres - 1) != (lenres - 1))
		exit(7);
		
	result[lenres] = '\0';
	
	return result;
}
