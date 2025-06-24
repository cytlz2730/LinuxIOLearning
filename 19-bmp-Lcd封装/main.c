#include "bmp.h"
#include <stdio.h>
#include "lcddevice.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/mman.h>


int main(void){
    
    struct LcdDevice* lcd = create_lcd("/dev/fb0");
    lcd->clear(0xffffff);
    struct Bmp *bmp = create_bmp("./pic.bmp");
    reverse(bmp);
    scale(bmp, 1280,800);
    lcd->draw_bmp(0,0,bmp);
    destroy_bmp(bmp);
    destroy_lcd();
}