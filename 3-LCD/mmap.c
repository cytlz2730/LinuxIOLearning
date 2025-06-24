#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/mman.h>

int main(void)
{
	int dev_fd=open("/dev/fb0",O_RDWR);
	if(dev_fd==-1)
	{
		perror("open-dev/fb0");
		return -1;
	}

	FILE *bmp_fp=fopen("fileio.bmp","r");
	if(bmp_fp==NULL)
	{
		perror("open-bmp_fp");
		return -1;
	}

	unsigned char * ptr=mmap(NULL,800*480*4,PROT_READ|PROT_WRITE,MAP_SHARED,dev_fd,0);
	if(ptr==MAP_FAILED)
	{
		perror("mmap");
		return -1;
	}

	fseek(bmp_fp,54,SEEK_SET);

	unsigned char *temp_ptr=ptr;
	for(int i=0;i<800*480;i++)
	{
		unsigned char rgb[3]={0};
		unsigned int data;
		fread(rgb,3,1,bmp_fp);
		memcpy(&data,rgb,3);

		memcpy(temp_ptr,&data,4);

		temp_ptr += 4;

	}

	munmap(ptr,800*480*4);
	close(dev_fd);
	fclose(bmp_fp);

	return 0;
}
