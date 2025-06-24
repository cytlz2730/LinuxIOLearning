#include "BMP.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <sys/mman.h>
#include <fcntl.h>

int main(void)
{
	int fd_dev = open("/dev/fb0",O_RDWR);
	if(fd_dev<0)
	{
		perror("open-fd_dev");
		return -1;
	}

	FILE *fp=fopen("fileio.bmp","r");
	if(fp==NULL)
	{
		perror("open-fp");
		return -1;
	}

	BMP *bmp=create(fp);

	printf("-----------------------------------\n");
	printf("-           Plese select          -\n");
	printf("-        0.none                   -\n");
	printf("-        1.only reverse           -\n");
	printf("-        2.only scale             -\n");
	printf("-        3.reverse and scale      -\n");
	printf("-----------------------------------\n");

	int sel;
	scanf("%d",&sel);
	int w,h;
	
	switch(sel)
	{
		case 1:
			bmp->reverse(bmp);
			break;
		case 2:
			printf("已知原图宽度和高度各为: %d, %d.\n",bmp->w,bmp->h);
			printf("请输入缩放后的宽度: ");
			scanf("%d",&w);
			printf("请输入缩放后的高度: ");
			scanf("%d",&h);
			bmp->scale(bmp,w,h);
			break;
		case 3:
			bmp->reverse(bmp);
			printf("已知原图宽度和高度各为: %d, %d.\n",bmp->w,bmp->h);
			printf("请输入缩放后的宽度: ");
			scanf("%d",&w);
			printf("请输入缩放后的高度: ");
			scanf("%d",&h);
			bmp->scale(bmp,w,h);
			break;
		default:
			break;
	}

	// int len=(bmp->pixelbyte)+1;
	unsigned char *ptr=mmap(NULL
										,(bmp->w)*(bmp->h)*((bmp->pixelbyte)+1)
										,PROT_READ|PROT_WRITE
										,MAP_SHARED
										,fd_dev
										,0);
	if(ptr==(void*)-1)
	{
		perror("ptr");
		return -1;
	}

	for(int j=0;j<(bmp->h);j++)
	{
		for(int i=0;i<(bmp->w);i++)
		{
			memcpy(ptr+(bmp->pixelbyte+1)*(i+j*(bmp->w))
						,(bmp->data)+(bmp->pixelbyte)*(i+j*(bmp->w))
						,3);
		}
	}

	munmap(ptr,(bmp->w)*(bmp->h)*(bmp->pixelbyte));
	
	bmp->destory(bmp);
	fclose(fp);
	close(fd_dev);
	return 0;
}
