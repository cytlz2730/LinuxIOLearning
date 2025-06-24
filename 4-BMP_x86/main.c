#include "BMP.h"

int main(void)
{
	FILE *fp_src=fopen("fileio.bmp","r");
	FILE *fp_dest=fopen("dest.bmp","w");

	BMP *bmp=create(fp_src);

	printf("-----------------------------------\n");
	printf("-  请选择以下操作并输入对应的序号 -\n");
	printf("-        0.都不要                 -\n");
	printf("-        1.只翻转                 -\n");
	printf("-        2.只缩放                 -\n");
	printf("-        3.翻转和缩放             -\n");
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

	bmp->destory(bmp,fp_src,fp_dest);

	fclose(fp_src);
	fclose(fp_dest);
	return 0;
}