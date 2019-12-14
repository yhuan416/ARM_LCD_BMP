#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include <errno.h>

#include <sys/types.h>
#include <sys/mman.h>

#include <math.h>

#include "bmp.h"

/**
 * 初始化lcd设备
 * @Author yHuan
 * @Date   2019-11-26
 * @param  device     设备名
 * @param  x_size     x大小
 * @param  y_size     y大小
 * @return            返回结构体指针
 */
int init_lcd(lcd_device_p lcd_device, const char *device, int x_size, int y_size)
{
	//打开设备
	int lcd_fd = open(device, O_RDWR);
	if (lcd_fd < 0)
	{
		perror("open lcd");
		return -1;
	}

	//映射内存地址
	int *lcd_mem = (int *)mmap(NULL, \
						x_size * y_size * 4, \
						PROT_READ|PROT_WRITE, \
						MAP_SHARED, \
						lcd_fd, \
						0\
					);

	//判断是否映射成功
	if (lcd_mem == NULL)
	{
		perror("mmap");
		close(lcd_fd);
		return -1;
	}

	//保存结构体参数
	lcd_device->lcd_x_size = x_size;
	lcd_device->lcd_y_size = y_size;
	lcd_device->lcd_mem = (unsigned int *)lcd_mem;

	//关闭文件
	close(lcd_fd);

	return 0;
}

/**
 * 关闭lcd屏
 * @Author yHuan
 * @Date   2019-08-18
 */
void close_lcd(lcd_device_p lcd_device)
{
	//判断
	if (lcd_device == NULL)
	{
		printf("lcd not init\n");
		return; 
	}

	if (lcd_device->lcd_mem == NULL)
	{
		printf("lcd not init\n");
		return;
	}

	//解除映射
	munmap(lcd_device->lcd_mem, (lcd_device->lcd_x_size)*(lcd_device->lcd_y_size)*4);
	lcd_device->lcd_mem = NULL;

	//清空结构体
	lcd_device->lcd_x_size = 0;
	lcd_device->lcd_y_size = 0;
}

//画方块
int draw_block_lcd(lcd_device_p lcd_device, int lcd_x, int lcd_y, int w, int h, int colour)
{
	if (lcd_device->lcd_mem == NULL)
	{
		printf("lcd need init!\n");
		return -1;
	}

	int x,y;
	int index;

	for(y = lcd_y; y < lcd_y + h; y++)
	{
		if (y >= (lcd_device->lcd_y_size) || y < 0){continue;}

		index = y * (lcd_device->lcd_x_size);

		for(x = lcd_x; x < lcd_x + w; x++)
		{		
			if (x >= (lcd_device->lcd_x_size) || x < 0){continue;}

			lcd_device->lcd_mem[index + x] = colour;
		}
	}
	return 0;		
}

//画点
int draw_point_lcd(lcd_device_p lcd_device, int lcd_x, int lcd_y, int r, int colour)
{
	if (lcd_device->lcd_mem == NULL)
	{
		printf("lcd need init!\n");
		return -1;
	}

	int x,y;
	int index;

	for(y = (lcd_y - r); y <= (lcd_y + r); y++)
	{
		if (y >= (lcd_device->lcd_y_size) || y < 0){continue;}

		index = y * (lcd_device->lcd_x_size);

		for(x = (lcd_x - r); x <= (lcd_x + r); x++)
		{		
			if (x >= (lcd_device->lcd_x_size) || x < 0){continue;}	

			if ((((double)x-lcd_x)*((double)x-lcd_x)) + (((double)y-lcd_y))*((double)y-lcd_y) <= (double)r*r)
			{
				lcd_device->lcd_mem[index + x] = colour;
			}
		}
	}	

	return 0;	
}

//画线
int draw_line_lcd(lcd_device_p lcd_device, int lcd_x, int lcd_y, int lcd_x1, int lcd_y1, int size, int colour)
{
	if (lcd_device->lcd_mem == NULL)
	{
		printf("lcd need init!\n");
		return -1;
	}

	//计算斜率
	double grad = (((double)(lcd_y1 - lcd_y))/(lcd_x1 - lcd_x));

	int x1, y1;//起始
	int x2, y2;//终止

	double half = size/2.0;

	if (lcd_x <= lcd_x1)//x轴遍历一遍
	{
		x1 = lcd_x;y1 = lcd_y;
		x2 = lcd_x1;y2 = lcd_y1;
	}
	else
	{
		x2 = lcd_x;y2 = lcd_y;
		x1 = lcd_x1;y1 = lcd_y1;
	}

	int i;
	for (i = x1; i < x2; ++i)
	{
		draw_block_lcd(lcd_device, i - half, ((grad*(i-x1)) + y1 - half), size, size, colour);
	}

	if (lcd_y <= lcd_y1)//y轴遍历一遍
	{
		x1 = lcd_x; y1 = lcd_y;
		x2 = lcd_x1; y2 = lcd_y1;
	}
	else
	{
		x2 = lcd_x; y2 = lcd_y;
		x1 = lcd_x1; y1 = lcd_y1;
	}

	for (i = y1; i < y2; ++i)
	{
		draw_block_lcd(lcd_device, (((i-y1)/grad) + x1 - half), i - half, size, size, colour);
	}
}

///画方形
int draw_rect_lcd(lcd_device_p lcd_device, int lcd_x, int lcd_y, int w, int h, int size, int colour)
{
	//通过画线画方形
	draw_line_lcd(lcd_device, \
		lcd_x + size/2, \
		lcd_y + size/2, \
		lcd_x + size/2, \
		lcd_y + h - size/2, \
		size, colour \
	);

	draw_line_lcd(lcd_device, \
		lcd_x + size/2, \
		lcd_y + h - size/2,  \
		lcd_x + w - size/2, \
		lcd_y + h - size/2, \
		size, colour \
	);

	draw_line_lcd(lcd_device, \
		lcd_x + w - size/2, \
		lcd_y + h - size/2, \
		lcd_x + w - size/2, \
		lcd_y + size/2, \
		size, colour \
	);

	draw_line_lcd(lcd_device, \
		lcd_x + w - size/2, \
		lcd_y + size/2,  \
		lcd_x + size/2, \
		lcd_y + size/2, \
		size, colour \
	);
}

//画圆
int draw_circle_lcd(lcd_device_p lcd_device, int lcd_x, int lcd_y, int r, int size, int colour)
{
	int temp, temp1 = 0;

	int y1, y2;//上一个点的位置
	y1 = lcd_y - r;

	for (y2 = y1; y2 <= lcd_y + r; ++y2)
	{
		temp = sqrt((r*r) - (y2-lcd_y)*(y2-lcd_y));

		draw_line_lcd(lcd_device, lcd_x + temp1, y1, lcd_x + temp, y2, size, colour);
		draw_line_lcd(lcd_device, lcd_x - temp1, y1, lcd_x - temp, y2, size, colour);

		temp1 = temp;
		y1 = y2;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

/* 回收图片信息结构体 */
int dele_bmp_info(bmp_info_p bmp_p)
{
	if (bmp_p == NULL)
	{
		printf("null point\n");
		return -1;
	}

	free(bmp_p->bmp_buf);
	free(bmp_p);

	return 0;
}

/**
 * 将bmp图片的信息读取进bmp结构体
 * @Author yHuan
 * @Date   2019-08-16
 * @param  name       bmp图片的名字
 * @return            执行正确:结构体指针
 */
bmp_info_p read_bmp(const char *name)
{
	if (name == NULL)
	{
		printf("name is null\n");
		return NULL;
	}

	//打开图片
	int bmp_fd = open(name, O_RDONLY);
	if (bmp_fd < 0)
	{
		perror("bmp_fd open");
		return NULL;
	}

	//图片大小信息
	int bmp_x_max, bmp_y_max;

	//读取图片信息
	lseek(bmp_fd, 18, SEEK_SET);

	//设置图片的格式
	read(bmp_fd, &bmp_x_max, 4);
	read(bmp_fd, &bmp_y_max, 4);

	//读取像素点信息
	char *bmp_buf = (char *)malloc(bmp_y_max * bmp_x_max * 3);
	memset(bmp_buf, 0, bmp_y_max * bmp_x_max * 3);

	int bmp_byte_per_line;

	//读取像素点信息
	if ((bmp_x_max * 3) % 4 == 0)
	{
		bmp_byte_per_line = 4 * (24 * bmp_x_max / 32);//每一行占的字节数(4字节对齐)
	}
	else
	{
		bmp_byte_per_line = 4 * ((24 * bmp_x_max / 32) + 1);//每一行占的字节数(4字节对齐)
	}

	for (int y = 0; y < bmp_y_max; ++y)
	{
		lseek(bmp_fd, 54 + (y*bmp_byte_per_line), SEEK_SET);
		read(bmp_fd, &bmp_buf[y*bmp_x_max*3], bmp_x_max * 3);
	}

	//关闭图片文件
	close(bmp_fd);

	//处理像素点变成AGBR格式
	int *lcd_buf = (int *)malloc(bmp_y_max * bmp_x_max * 4);
	for (int i = 0; i < (bmp_x_max * bmp_y_max); ++i)
	{
		lcd_buf[i] = (bmp_buf[(i*3)] | bmp_buf[(i*3) + 1] << 8| bmp_buf[(i*3) + 2] << 16 | 0x00 << 24);
	}

	//释放内存
	free(bmp_buf);

	//将图片信息存入结构体
	bmp_info_p bmp_p = (bmp_info_p)malloc(sizeof(bmp_info_t));
	if (bmp_p == NULL)
	{
		free(lcd_buf);
		return NULL;
	}

	bmp_p->x_size = bmp_x_max;
	bmp_p->y_size = bmp_y_max;
	
	bmp_p->bmp_buf = lcd_buf;

	return bmp_p;
}

/**
 * 显示图片
 * @Author yHuan
 * @Date   2019-12-13
 * @param  lcd_device lcd设备
 * @param  bmp_p      bmp图片
 * @param  lcd_x      图片显示的坐标
 * @param  lcd_y      
 * @param  bmp_w      图片显示的宽高
 * @param  bmp_h      
 * @param  offset_x   图片显示区域的偏移量
 * @param  offset_y   
 * @return            success on 0, error on -1
 */
int show_bmp(lcd_device_p lcd_device, bmp_info_p bmp_p, int lcd_x, int lcd_y, int bmp_w, int bmp_h, int offset_x, int offset_y)
{
	if (lcd_device->lcd_mem == NULL)
	{
		printf("lcd need init!\n");
		return -1;
	}

	if (bmp_p == NULL || bmp_p->bmp_buf == NULL)
	{
		printf("null point\n");
		return -1;
	}

	int lcd_index;
	int bmp_index;

	int x, y;

	int lcd_y_temp, lcd_x_temp;	

	int bmp_x_max, bmp_y_max;
	bmp_x_max = bmp_p->x_size;
	bmp_y_max = bmp_p->y_size;

	int *buf = bmp_p->bmp_buf;

	//显示图片
	for (y = offset_y; y < bmp_y_max && y < bmp_h + offset_y; ++y)
	{
		//判断是否超过上下边界
		lcd_y_temp = (lcd_y + y - offset_y);
		if (lcd_y_temp < 0 || lcd_y_temp >= (lcd_device->lcd_y_size)){continue;}
		
		//获取当前行的偏移值,方便后面赋值运算
		lcd_index = (lcd_y_temp * (lcd_device->lcd_x_size)) + lcd_x - offset_x;//屏幕的偏移量(这里减去图片的偏移是因为下面复制的时候计算进了图片的偏移,所以这里先减去)
		bmp_index = (bmp_y_max - 1 - y) * bmp_x_max;//图片的偏移量
		
		//遍历X轴
		for (x = offset_x; x < bmp_x_max && x < bmp_w + offset_x; ++x)
		{
			//判断X轴是否超过左右边界
			lcd_x_temp = (lcd_x + x - offset_x);
			if (lcd_x_temp < 0 || lcd_x_temp >= (lcd_device->lcd_x_size)){continue;}
			
			//循环赋值
			lcd_device->lcd_mem[lcd_index + x] = buf[bmp_index + x];
		}

	}

	return 0;
}

