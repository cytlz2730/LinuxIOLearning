#ifndef _BMP_H
#define _BMP_H

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

//BMP结构体
typedef struct BMP
{
	int w,h;
	int pixelbyte;
	unsigned char *data;
	// (BMP*) (*create)(int,int,int,FILE*);
	void (*reverse)(struct BMP*);
	void (*scale)(struct BMP*,int,int);
	void (*destory)(struct BMP *);
}BMP;

BMP* create_bmp(const char*);

#endif