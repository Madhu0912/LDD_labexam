#include<stdio.h>
#include<stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
	int fd;
	char kbuff[50];
	fd=open("chardevice",O_RDWR,0777);
	if(fd<0)
	{
		printf("error opeining file\n");
		exit(1);
	}
	read(fd,kbuff,50);
	printf("the data received from kernel is :%s\n",kbuff);
	close(fd);
}

