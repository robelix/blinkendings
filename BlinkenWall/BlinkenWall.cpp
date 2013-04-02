#include "BlinkenWall.h"

BlinkenWall::BlinkenWall(int default_delay)
{
	delay = default_delay;
}

int
BlinkenWall::transform(int pos)
{
	if (pos >= 96)
		return -1;
	int x = pos / 12;
	int y = pos % 12;
	return trans_mat[x][y];
}

void
BlinkenWall::setup()
{
	Tlc.init();
	Tlc.clear();
	clear();
}

void
BlinkenWall::clear()
{
	/* reset zbuffer */
	memset(zbuffer, 0xFF, 8 * 12);
}

void
BlinkenWall::drawAnimation(struct animation *ani, int xoffset, int yoffset, int zoffset, int frame)
{
	int pos;
	uint8_t f = 1 << frame;
	for (pos = 0; pos < 96; pos++) {
		int x = pos / 12;
		int y = pos % 12;
		int r,g,b;
		/* nothing to draw if it's hidden */
		if (zbuffer[x][y] < zoffset)
			return;
		int draw_pos = transform(pos);
		int col = 0;
		int ax = x - xoffset;
		int ay = y - yoffset;

		if (ax < ani->len_x && ay < ani->len_y && ax >= 0 && ay >= 0)
			col = ani->frames[(ax * ani->len_y) + ay];
		if (col & (int)f)
			zbuffer[x][y] = zoffset;

		/* TODO get color from animation */
		uint32_t colval = ani->color[(ax * ani->len_y) + ay];
		r = (int)(((colval & 0x00FF0000) >> 16) << MAX_COLOR_VAL);
		g = (int)(((colval & 0x0000FF00) >> 8) << MAX_COLOR_VAL);
		b = (int)(((colval & 0x000000FF)) << MAX_COLOR_VAL);

		if (col & (int)f)
			updateWall(draw_pos, r, g, b);
	}
}

void
BlinkenWall::update()
{
	update(delay);
}

void
BlinkenWall::update(int del)
{
	Tlc.update();
	::delay(del);
}

void
BlinkenWall::updateWall(TLC_CHANNEL_TYPE pos, uint16_t r, uint16_t g, uint16_t b)
{
	TLC_CHANNEL_TYPE tlc_channel = pos * COLORS;
	Tlc.set(tlc_channel + LED_R, r);
	Tlc.set(tlc_channel + LED_G, g);
	Tlc.set(tlc_channel + LED_B, b);
}

uint32_t *
BlinkenWall::createColor(uint32_t pattern, int lenx, int leny)
{
	int i;
	if (lenx < 1 || leny < 1)
		return NULL;
	uint32_t *color = (uint32_t *) malloc(lenx * leny);
	if (!color)
		return NULL;
	for (i = 0; i < lenx * leny; i++)
		color[i] = pattern;
	return color;
}
