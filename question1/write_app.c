#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
int main()
{
	int fd;
	int ubuff[50];
	fd=open("chardevice",O_RDWR,0777);
	if(fd<0)
	{
		printf("error opening file\n");
		exit(1);
	}
	write(fd,ubuff,sizeof(ubuff));
	close(fd);
}

