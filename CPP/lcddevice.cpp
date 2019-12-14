#include "lcddevice.h"

LcdDevice::LcdDevice(const char *fd, int lcdW, int lcdH)
{
    //open lcd device , mmap mem
    int ret = init_lcd(&this->lcddevice, fd, lcdW, lcdH);
    if (ret == 0)
    {
        this->isOpen = true;
    }
    else
    {
        this->isOpen = false;
    }
}

LcdDevice::~LcdDevice()
{
    //close lcd device ,unmap mem
    close_lcd(&this->lcddevice);
}

//show bmp at (0,0)
void LcdDevice::showBmp(const char *path)
{
    //read bmp and save bmp pixel in heap
    bmp_info_p bmp = read_bmp(path);

    show_bmp(&this->lcddevice, bmp, 0, 0, bmp->x_size, bmp->y_size, 0, 0);

    //free bmp
    dele_bmp_info(bmp);
}






