#include "common.h"

#include "../lib/bmp.h"

int main(int argc, char const *argv[])
{
	//init lcd device
	lcd_device_t lcd;
	init_lcd(&lcd, "/dev/fb0", 800, 480);
	
	//clean screen
	draw_block_lcd(&lcd, 0, 0, 800, 480, getColourRGB(0, 0, 0));
	
	//read bmp file
	bmp_info_p bmp = read_bmp("1.bmp");
	
	//show bmp at (0,0)
	show_bmp(&lcd, bmp, 0, 0, bmp->x_size, bmp->y_size, 0, 0);
	
	//delete bmp
	dele_bmp_info(bmp);
	
	//close lcd device
	close_lcd(&lcd);

	return 0;
}


