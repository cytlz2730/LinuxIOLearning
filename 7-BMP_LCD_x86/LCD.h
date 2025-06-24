#define DEBUG(x)  printf("%s-%d-%s\n", __FILE__, __LINE__, x)

#ifndef _LCD_H
#define _LCD_H

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <linux/fb.h>

#include "BMP.h"

typedef struct LCD
{
	int w,h;
	int vxres, vyres; //虚拟分辨率
	int pixelbyte;
	int fd; //文件描述符
	unsigned int *mptr; //映射空间首地址
	// 以下为函数指针	
	//销毁
	void (*destory)();
	//绘制图像
	void (*draw_bmp)(int,int,BMP*);
	//清屏
	void (*clear)(unsigned int);

}LCD;

LCD *create_lcd(const char*);

#endif