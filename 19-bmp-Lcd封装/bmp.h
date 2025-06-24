#ifndef _BMP_H
#define _BMP_H


struct Bmp{
    int w,h;
    int pixelbyte;//一个像素占用字节数（位深）
    unsigned char *data;//保存RGB数据流
};

struct Bmp*  create_bmp(const char *bmpfile);
void  destroy_bmp(struct Bmp* bmp);
//像素行反转
void  reverse(struct  Bmp* bmp);
//像素缩放
void  scale(struct Bmp* bmp, int w, int h);

#endif