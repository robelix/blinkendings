#ifndef __BLINKENWALL_H
#define __BLINKENWALL_H
#include "Arduino.h"
#include "tlc_config.h"
#include "Tlc5940.h"

/* TLC consts */
#define LED_R	0
#define LED_G	1
#define LED_B	2
#define COLORS	3

/* Maximum allowed color (brightness val)
 * 0 ... 255
 * 1 ... 512
 * 2 ... 1024
 * 3 ... 2048
 * 4 ... 4096
 */
#define MAX_COLOR_VAL 2

/* Transformation matrix for current wall-setup */
static const char trans_mat[8][12] =
{
	{16, 17, 18, 19, 32, 33, 34, 35, 80, 81, 82, 83},
	{20, 21, 22, 23, 36, 37, 38, 39, 84, 85, 86, 87},
	{24, 25, 26, 27, 40, 41, 42, 43, 88, 89, 90, 91},
	{28, 29, 30, 31, 44, 45, 46, 47, 92, 93, 94, 95},
	{ 0,  1,  2,  3, 48, 49, 50, 51, 64, 65, 66, 67},
	{ 4,  5,  6,  7, 52, 53, 54, 55, 68, 69, 70, 71},
	{ 8,  9, 10, 11, 56, 57, 58, 59, 72, 73, 74, 75},
	{12, 13, 14, 15, 60, 61, 62, 63, 76, 77, 78, 79}
};


/*
 * struct animation
 * This struct is used to define a simple animation
 * members:
 *		- num_frames ... number of frames of the animation
 *		- len_x, len_y .... size of the sprite
 *		- frames ... pointer to the sprite information (details see below)
 *		- color ... pointer to a color information for the sprite
 *
 * frames:
 * 		- allowed numbers 0-255
 *		- each frame is represented by a power of 2:
 *			+ 0 ... no color at all (transparent)
 *			+ 1 ... first frame
 *			+ 2 ... second frame
 *			+ 4 ... third frame
 *			...
 *			+ 128 ... eight frame
 *
 *			if a pixel should be displayed in multiple frames, the
 *			number of occuring frames must be summed
 *			e.g. 1st and 3rd frame = 1 + 4 = 5
 *			     2nd, 3rd, 4th, and 5th frame = 2 + 4 + 8 + 16 = 30
 * color:
 *		- each pixel can have a 3 * 8 bit color information
 *		- red: 0xFF0000
 *		- green: 0x00FF00
 *		- blue: 0x0000FF
 *		- every other color is in the format 0xRRGGBB -> RR ... red part
 *								 GG ... green part
 *								 BB ... blue part
 */

struct animation {
	int num_frames;
	int len_x;
	int len_y;
	const char *frames;
	const uint32_t *color;
};

class BlinkenWall {
public:
	BlinkenWall(int delay, uint32_t bg_color);
	void setup();
	void drawAnimation(struct animation *ani, int xoffset, int yoffset, int zoffset, int frame);
	void update();
	void update(int delay);
	void clear();
	uint32_t *createColor(uint32_t pattern, int lenx, int leny);
	void drawPoint(int x, int y, int z, uint32_t color);
	void setBackGround(uint32_t color);
private:
	void draw_raw(int pos, uint32_t color);
	void updateWall(TLC_CHANNEL_TYPE pos, uint16_t r, uint16_t g, uint16_t b);
	int transform(int pos);
private:
	int delay;
	uint32_t background;
	unsigned char zbuffer[8][12];
};

#endif /* __BLINKENWALL_H */
