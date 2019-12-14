#include "common.h"

#include "bmp.h"


int main(int argc, char const *argv[])
{
	if (argc != 2)
	{
		printf("arg error\n");
		return -1;
	}
	
	lcd_device_t lcd;
	init_lcd(&lcd, "/dev/fb0", 800, 480);

	draw_block_lcd(&lcd, 0, 0, 800, 480, getColourRGB(0, 0, 0));
	
	//draw_block_lcd(&lcd, 0, 0, 100, 100, getColourRGB(255, 0, 0));

	bmp_info_p bmp = read_bmp(argv[1]);
	
	/*
	
	int i = 0;
	
	for (i = 0; i <= 120; i++)
	{
		show_bmp(&lcd, bmp, 0, 0+i, 	800, 1, 0, 0+i);
		show_bmp(&lcd, bmp, 0, 120+i, 800, 1, 0, 120+i);
		show_bmp(&lcd, bmp, 0, 240+i, 800, 1, 0, 240+i);
		show_bmp(&lcd, bmp, 0, 360+i, 800, 1, 0, 360+i);
	
		usleep(5000);
	}
	
	getchar();
	
	for (i = 0; i <= 200; i++)
	{
		show_bmp(&lcd, bmp, i, 		0, 	1, 480, i, 		0);
		show_bmp(&lcd, bmp, i+200, 	0, 	1, 480, i+200, 	0);
		show_bmp(&lcd, bmp, i+400, 	0, 	1, 480, i+400, 	0);
		show_bmp(&lcd, bmp, i+600, 	0, 	1, 480, i+600, 	0);
	
		usleep(5000);
	}
	*/
	
	int i = 0, j = 0;
	
	int flag = 0;
	for(;;)
	{
		show_bmp(&lcd, bmp, i, 	j, 	40,	40, i, 	j);
		
		if (flag == 0)
		{
			i+=40;
		}
		else
		{
			i-=40;
		}
		
		
		if(i > 800)
		{
			flag = 1;
			j+=40;
		}
		else if(i < 0)
		{
			flag = 0;
			j+=40;
		}
		
		if(j > 480)
		{
			break;
		}
		
		usleep(5000);
	}
	
	printf("-------------------------------------------\n");
	
	
	
	
	
	
	
	
	
	
	
	
	
	dele_bmp_info(bmp);
	
	printf("-------------------------------------------\n");
	
	close_lcd(&lcd);

	return 0;
}


