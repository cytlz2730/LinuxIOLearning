#include "LCD.h"

// typedef struct LCD
// {
// 	int w,h;
// 	int pixelbyte;
// 	int fd; //文件描述符
// 	unsigned int *mptr; //映射空间首地址
// 	// 以下为函数指针	

// }LCD;

//一个设备只一个lcd
static LCD *instanceLcd = NULL;

// 销毁
static void destory();

// 绘制lcd
static void draw_bmp(int x,int y,BMP*);

//清屏
static void clear(unsigned int);

LCD *create_lcd(const char* filename)
{
	if(instanceLcd!=NULL) return instanceLcd;

	instanceLcd=malloc(sizeof(LCD));

	instanceLcd->fd = open(filename,O_RDWR);
	if(instanceLcd->fd<0)
	{
		free(instanceLcd);
		instanceLcd=NULL;
		perror("open-fd");
		return NULL;
	}

	struct fb_var_screeninfo info;
	ioctl(instanceLcd->fd,FBIOGET_VSCREENINFO,&info);
	printf("Screen resolution: %dx%d\n", info.xres, info.yres);
	printf("Pixel format: %d\n", info.bits_per_pixel);
	
	instanceLcd->w=info.xres;
	instanceLcd->h=info.yres;

	instanceLcd->vxres = info.xres_virtual;
	instanceLcd->vyres = info.yres_virtual;

	instanceLcd->pixelbyte=info.bits_per_pixel/8;

	instanceLcd->mptr=mmap(NULL
												,(instanceLcd->vxres)*(instanceLcd->vyres)*(instanceLcd->pixelbyte)
												,PROT_READ|PROT_WRITE
												,MAP_SHARED
												,instanceLcd->fd
												,0);

	instanceLcd->destory=destory;
	instanceLcd->draw_bmp=draw_bmp;
	instanceLcd->clear=clear;

	return instanceLcd;
}

//销毁
static void destory()
{
	if(instanceLcd==NULL)
		return;

	munmap(instanceLcd->mptr,(instanceLcd->vxres)*(instanceLcd->vyres)*(instanceLcd->pixelbyte));
	close(instanceLcd->fd);
	free(instanceLcd);
	instanceLcd=NULL;
}

// 绘制lcd
static void draw_bmp(int x,int y,BMP* bmp)
{
	// bmp的宽度和高度
	int bw = bmp->w,bh=bmp->h;

	// lcd的宽度和高度
	int lw=instanceLcd->w,lh=instanceLcd->h;

	// 实际上要绘制的宽度和高度
	int real_w = (x+bw)<=lw ? bw : (lw-x);
	int real_h = (y+bh)<=lh ? bh : (lh-y);

	// printf("real_w:%d,real_h:%d;lw:%d,lh:%d\n",real_w,real_h,lw,lh);

	// lcd开始绘制的地方
	unsigned int *lptr = instanceLcd->mptr + (x+y*instanceLcd->vxres);
	unsigned char *bptr = bmp->data;

	for(int i=0;i<real_h;i++)
	{
		for(int j=0;j<real_w;j++)
		{
			memcpy(lptr +j 
						, bptr +(j)*bmp->pixelbyte 
						, bmp->pixelbyte);

		}
		lptr += instanceLcd->vxres;
		bptr += bw*bmp->pixelbyte;
	}

}

//清屏
static void clear(unsigned int color)
{
	int total=instanceLcd->w*instanceLcd->h;
	for(int i=0;i<total;i++)
		memcpy(instanceLcd->mptr+i,&color,instanceLcd->pixelbyte);
}
