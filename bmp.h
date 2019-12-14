/*
 * Author : YHUAN
 * SHOW BMP LIB
 * 
 * Date : 2019.12.5
 * v1.2
 * 		修改了逻辑,可以比较方便的移植到C++的项目中
 * 		修复了一处有没有释放堆空间的问题
 * 		改善了指针的处理,更加严谨了
 *
 * Date : 2019.12.13
 * v1.3
 * 		修改了显示图片的函数,现在可以更方便的进行各种动态效果的实现
 * 		showbmp接口的参数更多了
 * 
 */
#ifndef BMP_H
#define BMP_H

#ifdef __cplusplus
extern "C"
{
#endif

//lcd设备信息结构体
typedef struct 
{
	int lcd_x_size;
	int lcd_y_size;

	unsigned int *lcd_mem;
} lcd_device_t, *lcd_device_p;

//bmp图片信息结构体
typedef struct {
	int x_size;
	int y_size;

	int *bmp_buf;
} bmp_info_t, *bmp_info_p;

/* 可以自定义指定的颜色ARGB值 */
#define getColourRGB(r,g,b) 	(0<<24|r<<16|g<<8|b)

/* 常用的宏函数 */
#define MAX(a,b)                ((a>b)?(a):(b))
#define MIN(a,b)                ((a<b)?(a):(b))
#define ABS(a)					((a>0)?(a):(-a))

//初始化设备////////////////////////////////////////////////////////////////////////////////////////////////////

/* 初始化/关闭lcd屏 */
int init_lcd(lcd_device_p lcd_device, const char *device, int x_size, int y_size);
void close_lcd(lcd_device_p lcd_device);

//画图模块//////////////////////////////////////////////////////////////////////////////////////////////////////

/* 画方点/圆点 */
int draw_block_lcd(lcd_device_p lcd_device, int lcd_x, int lcd_y, int w, int h, int colour);
int draw_point_lcd(lcd_device_p lcd_device, int lcd_x, int lcd_y, int r, int colour);

/* 画线 */
int draw_line_lcd(lcd_device_p lcd_device, int lcd_x, int lcd_y, int lcd_x1, int lcd_y1, int size, int colour);

/* 画方形 */
int draw_rect_lcd(lcd_device_p lcd_device, int lcd_x, int lcd_y, int w, int h, int size, int colour);

/* 画圆形 */
int draw_circle_lcd(lcd_device_p lcd_device, int lcd_x, int lcd_y, int r, int size, int colour);

//BMP模块///////////////////////////////////////////////////////////////////////////////////////////////////////

/* 读取/清空指定图片的数据到结构体中 */
bmp_info_p read_bmp(const char *name);
int dele_bmp_info(bmp_info_p bmp_p);

/* 根据传入的bmp结构体显示图片 */
int show_bmp(lcd_device_p lcd_device, bmp_info_p bmp_p, int lcd_x, int lcd_y, int bmp_w, int bmp_h, int offset_x, int offset_y);

#ifdef __cplusplus
}
#endif

#endif
