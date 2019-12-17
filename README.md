**2019.12.14**  
**Author : YHUAN**  
**这个BMP库是我自己写的,通过内存映射和文件IO对linux下的显示屏设备进行操作**  

# 目录简介:

*test.c 是在C语言环境下的测试文件*  

*lcddevice.cpp & lcddevice.h 是在C++中对库的封装*  
*main.cpp是C++环境下的测试文件*  

*libbmparm.so库是使用5.4.0版本的arm-linux-gcc编译的动态库*  


---


# 功能介绍:


## 自定义结构体:


### lcd设备信息结构体(后续可以扩展)
> lcd_device_t, lcd_device_p  

保存了当前显示屏的宽高,已经内存映射的首地址  
后续在屏幕上显示东西可以直接操作这个地址  

显示屏设备在映射地址结束后就被关闭,所以没有保存设备的文件描述符  

---

### bmp图片的信息结构体
> bmp_info_t, bmp_info_p  

包含了当前的bmp文件的宽高,已经文件的像素点(已经转化成ARGB的格式)  

如果是比较大的图片素材,最好在每一次读取后对其进行释放,否则会占用过多的堆空间  
如果是比较小的图片素材,可以在程序启动时进行加载,并在程序退出时进行释放,以达到提升程序运行效率的目的  


---

## LCD相关接口:


### 初始化/关闭显示屏,调用这两个函数对显示屏进行初始化或关闭
> int init_lcd(lcd_device_p lcd_device, const char *device, int x_size, int y_size);  
> void close_lcd(lcd_device_p lcd_device);  

	lcd_device:lcd结构体指针
	device:屏幕设备文件
	x_size:屏幕宽(像素点)
	y_size:屏幕高(像素点)

---

> 注意: 参数colour是32位的ARGB,可以通过宏函数getColourRGB(r,g,b)获取  

---


### 在显示屏上画圆点/方点(内部有填充)  
> int draw_block_lcd(lcd_device_p lcd_device, int lcd_x, int lcd_y, int w, int h, int colour);  
> int draw_point_lcd(lcd_device_p lcd_device, int lcd_x, int lcd_y, int r, int colour);  

	以上两个函数是直接操作内存进行填充上色的  

	w:方块的宽  
	h:方块的高  
	r:圆的半径  
	colour:填充的颜色  

	对于方块来说,基准点是方块的右上角  
	对于圆来说,基准点是圆心  


### 在显示屏上画线  
> int draw_line_lcd(lcd_device_p lcd_device, int lcd_x, int lcd_y, int lcd_x1, int lcd_y1, int size, int colour);  

	这个函数是调用上面的画方块函数完成的  

	lcd_x, lcd_y : 起始点  
	lcd_x1, lcd_y1 : 终止点  

	size:粗细(最好用偶数)  


### 在显示屏上画圆/矩形(内部无填充)
> int draw_rect_lcd(lcd_device_p lcd_device, int lcd_x, int lcd_y, int w, int h, int size, int colour);  
> int draw_circle_lcd(lcd_device_p lcd_device, int lcd_x, int lcd_y, int r, int size, int colour);  

	这两个函数都是都是调用上面的画圆点/方点函数完成的  
	参数接口与上面类似,就不一一介绍了  

---

> PS : clean screen can use draw_block_lcd();  
> example: draw_block_lcd(&lcd, 0, 0, lcd.lcd_x_size, lcd.lcd_y_size, getColourRGB(0,0,0));  

---


## BMP相关接口:

### 读取/释放bmp图片信息
> bmp_info_p read_bmp(const char *name);  
> int dele_bmp_info(bmp_info_p bmp_p);  

	name:图片路径
	bmp_p:图片结构体指针

### 显示图片
> int show_bmp(lcd_device_p lcd_device, bmp_info_p bmp_p, int lcd_x, int lcd_y, int bmp_w, int bmp_h, int offset_x, int offset_y);  

	lcd_device:lcd设备结构体指针  
	bmp_p:bmp图片结构体指针  


	lcd_x,lcd_y  
	图片显示在lcd的什么位置(左上角)  


	bmp_w,bmp_h  
	图片显示多大,如果这个值超出文件的真实宽高,则会显示图片的最大宽/高  


	offset_x,offset_y  
	图片上显示的区域与图片起始点的偏移量  


---  

