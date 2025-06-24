#include <stdio.h>
#include "bmp.h"
#include <stdlib.h>
#include <string.h>
typedef short int16_t;
typedef int int32_t;
struct bitmap_header
{
	int16_t type;
	int32_t fsize; // 图像文件大小
	int16_t reserved1;
	int16_t reserved2;
	int32_t offbits; // bmp图像数据偏移量
	int32_t size;   // 本结构大小	
	int32_t width;  // 图像宽
	int32_t height; // 图像高
	int16_t planes;
	int16_t bit_count; // 色深
	int32_t compression;
	int32_t size_img; // bmp数据大小，必须是4的整数倍
	int32_t X_pel;
	int32_t Y_pel;
	int32_t clrused;
	int32_t clrImportant;
}__attribute__((packed)); //1字节对齐


struct Bmp*  create_bmp(const char *bmpfile){
    //打开文件
    FILE *file  = fopen(bmpfile, "r");
    if(file == NULL){
        perror("create_bmp");
        return NULL;
    }
    //读取图片头
    struct bitmap_header header;
    size_t ret = fread(&header, 1, sizeof(header), file);
    if(ret != sizeof(header))
    {
        perror("read  header");
        fclose(file);
        return NULL;
    }

    //创建bmp图片结构体
    struct Bmp*  bmp = malloc(sizeof(struct Bmp));
    if(bmp == NULL){
        perror("bmp malloc");
    }
    bmp->w = header.width;
    bmp->h = header.height;
    bmp->pixelbyte = header.bit_count/8;

    //申请存储RGB像素的空间
    bmp->data = malloc(bmp->w*bmp->h*bmp->pixelbyte);

    //读取bmp文件中的像素数据(按一行一行读取)
    int linesize = bmp->w*bmp->pixelbyte; //实际一行像素占用的字节数
    int filelinesize = bmp->w*bmp->pixelbyte;  //文件中一行像素字节数
    if(linesize % 4 != 0){
        filelinesize += 4-(linesize%4);
    }

    for(int i=0; i<bmp->h; i++){
        //定义一个临时存储一行像素的空间（读取文件）
        unsigned char filelinebuffer[filelinesize];
        fread(filelinebuffer,1, filelinesize, file);
        //把读取到的像素拷贝到data中，并且把补齐部分数据丢弃
        memcpy(bmp->data+i*linesize, filelinebuffer, linesize);
    }

    fclose(file);
    return bmp;
}

void  destroy_bmp(struct Bmp* bmp)
{
    if(bmp != NULL){
        if(bmp->data !=NULL){
            free(bmp->data);
        }
        free(bmp);
    }
}

void  reverse(struct  Bmp* bmp)
{
    if(bmp == NULL) return;
    //保存一行像素空间
    unsigned char linebuffer[bmp->w*bmp->pixelbyte];
    unsigned int linesize = bmp->w*bmp->pixelbyte;//一行占用的字节数
    
    unsigned char* start=bmp->data;
    unsigned char* end = bmp->data + (bmp->h-1) * linesize;
    for(int i=0; i<bmp->h/2; i++){
         memcpy(linebuffer, start, linesize);
         memcpy(start, end, linesize);
         memcpy(end, linebuffer, linesize); 
         start += linesize;
         end -= linesize;           
    }    
}

//缩放
void  scale(struct Bmp* bmp, int w, int h)
{
    if(bmp == NULL) return ;

    //定义一个空间存储缩放后的像素数据
    unsigned char* destData = malloc(w*h*bmp->pixelbyte);

    //定义缩放宽高比例
    double dw = bmp->w*1.0 / w;
    double dh = bmp->h*1.0 / h;

    //把源数据bmp->data拷贝到目标destData里面
    int pty = bmp->pixelbyte;//一个像素占用字节数

    for(int j=0; j<h; j++){
        for(int i=0; i<w; i++){
            memcpy(destData+i*pty + j*w*pty, bmp->data+((int)(i*dw))*pty + ((int)(j*dh))*bmp->w*pty, pty);
        }
    }

    //把原图数据释放
    free(bmp->data);
    bmp->w = w;
    bmp->h = h;
    bmp->data = destData;
}