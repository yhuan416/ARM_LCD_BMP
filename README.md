2019.12.14
 
Author : YHUAN
 
这个BMP库是我自己写的,通过内存映射和文件IO对linux下的显示屏设备进行操作.

libbmparm.so库是使用5.4.0版本的arm-linux-gcc编译的动态库

这个库实现了画点/方块,以及画圆/矩形的功能

同时,还附带了bmp图片的显示
int show_bmp(lcd_device_p lcd_device, bmp_info_p bmp_p, int lcd_x, int lcd_y, int bmp_w, int bmp_h, int offset_x, int offset_y);
 
其中:
lcd_x, lcd_y 是确定图片显示在lcd屏幕的那个位置
bmp_w, bmp_y 是确定图片显示多大(如果这个值超出图片本身的大小,则显示整张图片)
offset_x, offset_y 可以选择该图片显示时的偏移量,从而只显示图片的一部分

