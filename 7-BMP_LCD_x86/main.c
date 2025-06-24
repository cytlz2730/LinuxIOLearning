#include "BMP.h"
#include "LCD.h"

int main(void)
{
	LCD *lcd=create_lcd("/dev/fb0");
	if(lcd==NULL)
		return -1;
	BMP *bmp=create_bmp("fileio.bmp");
	if(bmp==NULL)
		return -1;

	printf("-----------------------------------\n");
	printf("-        Enter select for bmp     -\n");
	printf("-        0.none                   -\n");
	printf("-        1.only bmp reverse       -\n");
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
			printf("The original image width and height: %d, %d.\n",bmp->w,bmp->h);
			printf("Enter scaled weight: ");
			scanf("%d",&w);
			printf("Enter scaled height: ");
			scanf("%d",&h);
			bmp->scale(bmp,w,h);
			break;
		case 3:
			bmp->reverse(bmp);
			printf("The original image width and height: %d, %d.\n",bmp->w,bmp->h);
			printf("Enter scaled weight: ");
			scanf("%d",&w);
			printf("Enter scaled height: ");
			scanf("%d",&h);
			bmp->scale(bmp,w,h);
			break;
		default:
			break;
	}

	int x=0,y=0;
	printf("Enter the offset to write to the device:\n");
	printf("X offset: ");
	scanf("%d",&x);
	printf("Y offset: ");
	scanf("%d",&y);

	unsigned int color;
	unsigned char rgb[3]={0};
	
	printf("-----------------------------------\n");
	printf("-   Enter select for clear RGB:   -\n");
	printf("-        1.White                  -\n");
	printf("-        2.Black                  -\n");
	printf("-        3.Custom                 -\n");
	printf("-----------------------------------\n");
	scanf("%d",&sel);
	switch(sel)
	{
		case 1:
			rgb[0]=rgb[1]=rgb[2]=255;
			break;
		case 2:
			rgb[0]=rgb[1]=rgb[2]=0;
			break;
		case 3:
			printf("Enter Red: ");
			scanf("%hhu",&rgb[2]);
			printf("Enter Green: ");
			scanf("%hhu",&rgb[1]);
			printf("Enter Blue: ");
			scanf("%hhu",&rgb[0]);
			break;
	}

	memcpy(&color,rgb,3);

	lcd->clear(color);
	lcd->draw_bmp(x,y,bmp);
	
	lcd->destory();
	bmp->destory(bmp);
	
	sleep(60);

	return 0;
}
