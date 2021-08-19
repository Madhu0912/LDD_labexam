#include<stdio.h>
#include<stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
struct stats
{
	int size;
	char buff[20];
	int r_w;
int main()
{
	int fd;
	fd = open("ioctl",O_RDWR,0777);
	if(fd<0)
	{
		printf("error opening file\n");
		exit(1);
	}
	ioctl(fd,GETSTATS,&stats);
	printf("enter string:");
	scanf("%s",&stats.buff);
}
