#ifndef _LCDDEVICE_H
#define _LCDDEVICE_H
#include "bmp.h"
struct LcdDevice {
    int w, h;
    int pixelbyte;
    int fd;//文件描述符
    unsigned int *mptr; //映射空间首地址
    void (*draw_bmp)(int x, int y, struct Bmp *bmp);
    //清屏函数
    void (*clear)(unsigned int color);
    //画线直线
    void (*draw_line)(int x1, int y1, int x2, int y2);
    //画圆
    void (*draw_cirlce)(int x, int y, int r);
    //画矩形
    void (*draw_rect)(int x, int y, int w, int h);
};

struct LcdDevice* create_lcd(const char *devname);
void destroy_lcd();



#endif 