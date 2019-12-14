#ifndef LCDDEVICE_H
#define LCDDEVICE_H

#include "lib/bmp.h"

class LcdDevice
{
private:
    lcd_device_t lcddevice;
    bool isOpen;

public:
    LcdDevice(const char *fd = "/dev/fb0", int lcdW = 800, int lcdH = 480);
    virtual ~LcdDevice(){}

    virtual void showBmp(const char *path);
};

#endif // LCDDEVICE_H
