#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

int main(void)
{
    int dev_fd=open("/dev/fb0",O_WRONLY);
    if(dev_fd==-1)
    {
        perror("open-dev_fd");
        return -1;
    }

    FILE *bmp_fp=fopen("fileio.bmp","r");
    if(bmp_fp==NULL)
    {
        perror("open-bmp_fp");
        return -1;
    }

    fseek(bmp_fp,54,SEEK_SET);

    for(int i=0;i<800*480;i++)
    {
        unsigned char rgb[3]={0};
        unsigned int data;
        fread(rgb,3,1,bmp_fp);
        memcpy(&data,rgb,3);
        write(dev_fd,&data,4);
    }

    close(dev_fd);
    fclose(bmp_fp);
    return 0;
}