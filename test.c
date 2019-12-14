#include "common.h"

#include "lib/bmp.h"


int main(int argc, char const *argv[])
{
	lcd_device_t lcd;
	init_lcd(&lcd, "/dev/fb0", 800, 480);

	draw_block_lcd(&lcd, 0, 0, 800, 480, getColourRGB(0, 0, 0));
	
	
	bmp_info_p bmp = read_bmp("1.bmp");
	show_bmp(&lcd, bmp, 0, 0, bmp->x_size, bmp->y_size, 0, 0);
	
	
	dele_bmp_info(bmp);
	close_lcd(&lcd);

	return 0;
}


