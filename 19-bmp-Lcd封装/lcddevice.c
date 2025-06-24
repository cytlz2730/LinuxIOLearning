#include "lcddevice.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <stdio.h>
//一个设备只一个lcd， 
static struct LcdDevice *instanceLcd = NULL;


void clear(unsigned int color)
{
    for(int i=0; i<instanceLcd->h; i++)
    {
        for(int j=0; j<instanceLcd->w; j++){
            instanceLcd->mptr[j+i*instanceLcd->w] = color;
        }
    }
}


static void draw_bmp(int x, int y, struct Bmp *bmp){

    //考虑图部分在屏幕内，部分在屏幕外面, 计算真实要绘制的区域
    int dw =  ((x+bmp->w)<=instanceLcd->w)?bmp->w:(instanceLcd->w - x); 
    int dh =  ((y+bmp->h)<=instanceLcd->h)?bmp->h:(instanceLcd->h - y); 
    //定义一个指针移动到绘制开始位置 x，y
    unsigned int *p = instanceLcd->mptr + y*instanceLcd->w+x;
    unsigned char *q = bmp->data;
    //绘制图片
    for(int i=0; i<dh; i++){
        for(int j=0; j<dw; j++){
            memcpy(p+j, q+j*bmp->pixelbyte, bmp->pixelbyte);
        }
        //lcd要移动到下一行
        p = p+instanceLcd->w;
        //图片数据指针也要移动到下一行
        q = q+bmp->w*bmp->pixelbyte;
    }
}

struct LcdDevice* create_lcd(const char *devname){
    if(instanceLcd != NULL) return instanceLcd;
    //申请lcd结构体空间
    instanceLcd = (struct LcdDevice*)malloc(sizeof(struct LcdDevice));
    if(instanceLcd == NULL){
        return NULL;
    }
    //打开lcd
    instanceLcd->fd = open(devname, O_RDWR);
    if(instanceLcd->fd < 0){
        free(instanceLcd);
        instanceLcd = NULL;
        return NULL;
    }
    //获取lcd属性（ioctl）---分辨率，色深
    struct fb_var_screeninfo info;
    int ret = ioctl(instanceLcd->fd, FBIOGET_VSCREENINFO, &info);
    if(ret < 0){
        perror("获取lcd属性");
    }
    instanceLcd->w = info.xres;
    instanceLcd->h = info.yres;
    instanceLcd->pixelbyte = info.bits_per_pixel/8;
    
    printf("%d:%d:%d\n", instanceLcd->w, instanceLcd->h, instanceLcd->pixelbyte);
    //映射
    instanceLcd->mptr = (unsigned int *)mmap(NULL, instanceLcd->w*instanceLcd->h*instanceLcd->pixelbyte, 
                           PROT_READ|PROT_WRITE, MAP_SHARED,instanceLcd->fd, 0);
    if(instanceLcd->mptr == MAP_FAILED)
    {
        perror("mmap");
        return NULL;
    }

    //初始化成员函数
    instanceLcd->draw_bmp = draw_bmp;
    instanceLcd->clear = clear;
    return instanceLcd;
}

void destroy_lcd(){
    if(instanceLcd != NULL)
    {
        //释放映射
        munmap(instanceLcd->mptr, instanceLcd->w*instanceLcd->h*instanceLcd->pixelbyte);
        //关闭文件
        close(instanceLcd->fd);
        //free
        free(instanceLcd);
        instanceLcd = NULL;
    }
}