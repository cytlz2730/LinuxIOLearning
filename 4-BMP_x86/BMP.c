#include "BMP.h"

// BMP结构体
//  tyoedef struct BMP
//  {
//  	int w,h;
//  	int pixelbyte;
//  	unsigned char *data;
//  	// (BMP*) (*create)(int,int,int,FILE*);
//  	void (*reverse)(BMP*);
//  	void (*scale)(BMP*,int,int);
//  	void (*destory)(BMP *,FILE*);
//  }BMP;

// 翻转
static void reverse(BMP *bmp);
// 最近邻插值
static void scale(BMP *bmp, int w, int h);
// 销毁前将数据存储在新文件
static void destory(BMP *bmp, FILE *fp_old, FILE *fp_new);

// 创建
BMP *create(FILE *fp)
{
    unsigned char header[54];
    fseek(fp, 0, SEEK_SET);
    fread(header, 1, 54, fp);

    //宽度unsigned int 18,高度unsigned int 22,位深short 28
    int w = *((int *)&header[18]);
    int h = *((int *)&header[22]);
    int pixelBit = *((short *)&header[28]);
    int pixelbyte = pixelBit / 8;

    BMP *bmp = malloc(sizeof(BMP));
    bmp->data = malloc(w * h * pixelbyte);

    bmp->w = w;
    bmp->h = h;
    bmp->pixelbyte = pixelbyte;

    bmp->reverse = reverse;
    bmp->scale = scale;
    bmp->destory = destory;

    int real_w_byte =(((bmp->pixelbyte)*(bmp->w)+3)/4)*4;

    for (int i = 0; i < h; i++) {
        fread(bmp->data + i * w * pixelbyte, pixelbyte, w, fp);
        fseek(fp, real_w_byte - w * pixelbyte , SEEK_CUR);
    }

    return bmp;
}


// 翻转
static void reverse(BMP *bmp)
{
	int j = (bmp->h) - 1;
	int len = (bmp->w) * (bmp->pixelbyte);
	for (int i = 0; i < (bmp->h) / 2; i++)
	{
		unsigned char temp[(bmp->w) * (bmp->pixelbyte)];
		memcpy(temp, (bmp->data) + i * len, len);
		memcpy((bmp->data) + i * len, (bmp->data) + j * len, len);
		memcpy((bmp->data) + j * len, temp, len );
		j--;
	}
}

// 缩放
static int get_coord(int i,float ratio,int max)
{
	int res = (int)(i * ratio+0.5f);
	return (res >= max) ? (max - 1) : res;
}

// 最近邻插值
static void scale(BMP *bmp, int w, int h)
{
	float xRatio = (float)(bmp->w)/w;
	float yRatio = (float)(bmp->h)/h;

	unsigned char *new_data=malloc(w*h*(bmp->pixelbyte));
	for(int j=0;j<h;j++)
	{
		for(int i=0;i<w;i++)
		{
			int new_x=get_coord(i,xRatio,bmp->w);
			int new_y=get_coord(j,yRatio,bmp->h);
			//拷贝(i,j)位置的像素，还要考虑位深
			memcpy(new_data+(i+j*w)*(bmp->pixelbyte)
						,(bmp->data)+(new_x+new_y*(bmp->w))*(bmp->pixelbyte)
						,bmp->pixelbyte);
		}
	}
	free(bmp->data);
	
	bmp->data=new_data;	

	bmp->w=w;
	bmp->h=h;
}

// 销毁前将数据存储在新文件
static void destory(BMP *bmp, FILE *fp_old, FILE *fp_new)
{
    unsigned char header[54];
    fseek(fp_old, 0, SEEK_SET);
    fread(header, 1, 54, fp_old);

    // 修改宽、高字段
    *(int *)&header[18] = bmp->w;
    *(int *)&header[22] = bmp->h;

    // 重新计算图像数据大小（注意每行 4 字节对齐）
    // 图像大小34,文件大小2
    int row_size = (((bmp->w)*(bmp->pixelbyte)+3)/4)*4;
    *(int *)&header[34] = row_size*(bmp->h);
    *(int *)&header[2] =  row_size*(bmp->h) + 54;

    fwrite(header, 1, 54, fp_new);

    // 写入像素数据（带填充）
    for (int i = 0; i < bmp->h; i++)
    {
        fwrite(bmp->data + i * bmp->w * bmp->pixelbyte, bmp->pixelbyte, bmp->w, fp_new);
        int padding = row_size - bmp->w * bmp->pixelbyte;
        if (padding > 0)
        {
            unsigned char pad[3] = {0}; // 最多填充 3 字节
            fwrite(pad, 1, padding, fp_new);
        }
    }

    free(bmp->data);
    free(bmp);
}


