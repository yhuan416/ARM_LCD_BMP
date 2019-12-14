#include <iostream>
#include "lcddevice.h"

using namespace std;

int main()
{
    LcdDevice lcd;

	//show bmp
    lcd .showBmp("1.bmp");

    return 0;
}
