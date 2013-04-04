/*
 * Space Invader for Blinkenwall
 *
 * using 6 panels a 16 LEDS
 */

#include "tlc_config.h"
#include "Tlc5940.h"

/* offset for colors */
#define LED_R	0
#define LED_G	1
#define LED_B	2

#define COLORS	3

static const char trans_mat[8][12]
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

static const char inv[] = 
{
	0, 0, 3, 0, 0, 0, 0, 0, 3, 0, 0,
	2, 0, 0, 3, 0, 0, 0, 3, 0, 0, 2,
	2, 0, 3, 3, 3, 3, 3, 3, 3, 0, 2,
	2, 3, 3, 0, 3, 3, 3, 0, 3, 3, 2,
	1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 1,
	1, 0, 3, 3, 3, 3, 3, 3, 3, 0, 1,
	1, 0, 3, 0, 0, 0, 0, 0, 3, 0, 1,
	0, 2, 0, 1, 1, 0, 1, 1, 0, 2, 0
};

static const char inv2[] =
{
	0, 0, 0, 3, 3, 0, 0, 0,
	0, 0, 3, 3, 3, 3, 0, 0,
	0, 3, 3, 3, 3, 3, 3, 0,
	3, 3, 0, 3, 3, 0, 3, 3,
	3, 3, 3, 3, 3, 3, 3, 3,
	0, 2, 1, 2, 2, 1, 2, 0,
	2, 1, 0, 1, 1, 0, 1, 2,
	1, 2, 1, 0, 0, 1, 2, 1
};


static const char pman[] = {
    0, 0, 7, 7, 7, 7, 0, 0,
    0, 7, 7, 7, 7, 7, 7, 0,
    7, 7, 7, 7, 0, 7, 6, 6,
    7, 7, 7, 7, 7, 4, 4, 4,
    7, 7, 7, 7, 7, 7, 6, 6,
    0, 7, 7, 7, 7, 7, 7, 0,
    0, 0, 7, 7, 7, 7, 0, 0
};

static const char tnl[] = {
	0, 64, 32, 16, 8, 8, 8, 16, 32, 64, 0,
	64, 32, 16, 8, 4, 4, 4, 8, 16, 32, 64,
	32, 16, 8, 4, 0, 2, 0, 4, 8, 16, 32,
	32, 16, 8, 4, 2, 1, 2, 4, 8, 16, 32,
	32, 16, 8, 4, 0, 2, 0, 4, 8, 16, 32,
	64, 32, 16, 0, 4, 4, 4, 8, 16, 32, 64,
	0, 64, 32, 16, 8, 8, 8, 16, 32, 64, 0,
	0, 0, 64, 32, 16, 16, 16, 32, 64, 0, 0
};

static const char andi[] = {
        0,1,2,0,0,0,6,4,0,0,8,
	1,0,3,2,0,0,6,0,4,0,8,
	1,1,3,0,2,0,6,0,4,0,8,
	1,0,3,0,0,2,6,0,4,0,8,
	1,0,3,0,0,0,6,4,0,0,8
};
/*
static const char cpunx[] = {
        0,1,3,2,4,0,28,0,0,0,24,
	1,0,2,0,6,0,12,24,0,16,8,
	1,0,2,2,4,0,12, 0,24,0,8,
	1,0,2,0,4,0,12,16,0,24,8,
	0,1,3,0,0,4,24, 0,0,0,24
};
*/

static const char gameover[] = {
	0,0,0,1,0,0,0,1,0,0,0,0,
	0,0,3,3,3,0,3,3,3,0,0,0,
	0,7,7,7,7,7,7,7,7,7,0,0,
	0,15,15,15,15,15,15,15,15,15,0,0,
	0,0,31,31,31,31,31,31,31,0,0,0,
	0,0,0,63,63,63,63,63,0,0,0,0,
	0,0,0,0,127,127,127,0,0,0,0,0,
	0,0,0,0,0,127,0,0,0,0,0,0
};

static const char cpunx[] = {
        0,1,5,4,8,0,56,0,0,0,48,
	1,0,4,0,12,0,24,48,0,32,16,
	1,2,6,4,8,0,24, 0,48,0,16,
	1,0,4,0,8,0,24,32,0,48,16,
	0,1,5,0,0,8,48, 0,0,0,48
};


static const char its[] = {
        1,1,1,2,2,2,2,2,0,4,4,
        0,1,0,0,0,2,0,0,4,0,0,
        0,1,0,0,0,2,0,0,0,4,0,
        0,1,0,0,0,2,0,0,0,0,4,
        1,1,1,0,0,2,0,0,4,4,0
};

static const char rkt[] = {
	2,0,0,15,15,0,0,0,0,0,0,0,
	4,2,0,15,15,15,15,15,15,15,0,0,
	8,4,2,15,15,15,15,15,15,15,15,0,
	0,8,4,2,15,15,15,15,15,15,15,15,
	8,4,2,15,15,15,15,15,15,15,15,0,
	4,2,0,15,15,15,15,15,15,15,0,0,
	2,0,0,15,15,0,0,0,0,0,0,0
};


static const char bg[] = {
	1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1
};

static unsigned char zbuffer[8][12];

struct animation {
	int frames;
	int x;
	int y;
        int invert;
	const char *data; 
};

struct animation invader1 = {2, 8, 11, 0, inv};
struct animation invader2 = {2, 8, 8, 0, inv2};
struct animation pacman = {3, 7, 8, 0, pman};
struct animation tunnel = {7, 8, 11, 0, tnl};
struct animation andimation = {4, 5, 11, 0, andi};
struct animation cpunxmation = {6, 5, 11, 0, cpunx};
struct animation itsmation = {3, 5,11, 0, its};
struct animation rocket = {4, 7, 12, 0, rkt};
struct animation backgroud = {1, 8, 12, 0, bg};
struct animation go = {8, 8, 12, 0, gameover};

/* set color */
void
tlc_setColor(TLC_CHANNEL_TYPE channel, uint16_t r, uint16_t g, uint16_t b) {
	TLC_CHANNEL_TYPE tlc_channel = channel * COLORS;
	/* set rgb values */
	Tlc.set(tlc_channel + LED_R, r);
	Tlc.set(tlc_channel + LED_G, g);
	Tlc.set(tlc_channel + LED_B, b);
}

void
clearZ()
{
	int x,y;
	for (x = 0; x < 8; x++)
		for(y = 0; y < 12; y++)
			zbuffer[x][y] = 0xff;
}

void
setup()
{
	Tlc.init();
	Tlc.clear();
        clearZ();
}


/* position transform for our actual setup */
int
transform(int pos)
{
	/* panel setup
	  1 2 5
	  0 3 4

	  0,0 -> 16
	 */
	if (pos >= 96)
		return -1;
	int x = pos / 12;
	int y = pos % 12;
	return trans_mat[x][y];
}

void
draw (struct animation ani, int pos, int r, int g, int b, int offset, int xoffset, int zoffset, float zoom, int frame)
{
	int x = pos / 12;
	int y = pos % 12;
	int draw_pos = transform(pos);
	int col = 0;
        int ay = y - offset;
	int ax = x - xoffset;
	if (zbuffer[x][y] < zoffset)
		return;
	ay *= zoom;
	ax *= zoom;

	if (ax < ani.x && ay < ani.y && ay >= 0 && ax >= 0)
		col = ani.data[(ax*ani.y) + ay];	
	else
		col = 0;
        if (col & frame)
	  zbuffer[x][y] = zoffset;

	if (col & frame)
		tlc_setColor(draw_pos, r, g, b);
}

void
loop()
{
	/* main loop */
	static int pos = 0;
	static int offset = 0;
	static int offset1 = -12;
	static int up = 1;
	static int lc = 0;
	static int a = 1;
        static int ro = 0;
	static int frame = 1;
	static int frame2 = 4;
        static int frame3 = 16;
	static int pacframe = 1;
        static int andif = 1;
        static int rocketf = 1;
        static int dly = 200;
        static int gof = 1;
	static struct animation *ani = &invader1;
	static struct animation *ani1 = &invader2;
	static struct animation *ani2 = &pacman;
        static int love = 1;
	for (pos = 0; pos < 96; pos++) {
		//draw(*ani2, pos, 1024, 1024, 0, offset, 1, 1, 1, pacframe);
		draw(backgroud, pos, 0, 0, 0, 0, 0, 250, 1, 1);
		
		
	        switch(a) {	
		case 6:
			draw(*ani, pos, 0, 1024, 0, offset, 0, 1, 1,  up); 
			draw(*ani1, pos, 0, 1024, 1024, offset1, 0, 1, 1, up == 1 ? 2 : 1);
                        dly = 200;
                        break;
                        /*
                case 2:
                        draw(cpunxmation, pos, 1024, 1024, 1024, 0, 2, 1, 1, andif);
                        dly = 350;
                        break;
                        */
                case 2:
                        draw(go, pos, 1024,0,0,0,0,1,1,gof);
                        dly = 150;
                        break;
                case 1:
                        draw(go, pos, 1024, 0,0,0,0,2,1,gof);
                        draw(itsmation, pos, 1024, 768, 768, 0,2,1,1,pacframe);
                        dly = 150;
                        break;
                case 4:
                        draw(rocket, pos, 1024, 1024, 1024, offset, 0, 1, 1, rocketf);
                        dly = 100;
                        break;
                case 3:
                case 5:
                case 7:
		        draw(tunnel, pos, 0, 16 * frame, 0, 0, 0, 1, 1, frame);
		        draw(tunnel, pos, 0, 16 * frame2, 0, 0, 0, 2, 1, frame2);
		        draw(tunnel, pos, 0, 16 * frame3, 0, 0, 0, 2, 1, frame3);
                        dly = 90;
                        break;
                case 8:
  			draw(*ani2, pos, 1024,1024,0, offset, 1, 1, 1, pacframe);
                        dly = 120;
                        break;
		} 
	}
	switch(pacframe){
	case 1: pacframe=2; break;
	case 2: pacframe=4; break;
	case 4: pacframe=1; break;
	}
        switch(andif){
        case 1: andif=2; break;
        case 2: andif=4; break;
        case 4: andif=8; break;
        case 8: andif=16; break;
        case 16: andif=32; break;
        case 32: andif=1; break;
        }
        switch(gof){
        case 1: gof=2; break;
        case 2: gof=4; break;
        case 4: gof=8; break;
        case 8: gof=16; break;
        case 16: gof=32; break;
        case 32: gof=64; break;
        case 64: gof=128; break;
        case 128: gof=256; break;
        case 256: gof=1; break;
        }

        switch(rocketf) {
        case 1: rocketf=2; break;
        case 2: rocketf=4; break;
        case 4: rocketf=8; break;
        case 8: rocketf=1; break;
        }
	Tlc.update();
	delay(dly);
        //Tlc.clear();
        clearZ();
	switch(frame) {
	case 1: frame=2; break;
	case 2: frame=4; break;
	case 4: frame=8; break;
	case 8: frame=16; break;
	case 16: frame=32; break;
	case 32: frame=64; break;
	case 64: frame=1; break;
	}
	switch(frame2) {
	case 1: frame2=2; break;
	case 2: frame2=4; break;
	case 4: frame2=8; break;
	case 8: frame2=16; break;
	case 16: frame2=32; break;
	case 32: frame2=64; break;
	case 64: frame2=1; break;
	}
	switch(frame3) {
	case 1: frame3=2; break;
	case 2: frame3=4; break;
	case 4: frame3=8; break;
	case 8: frame3=16; break;
	case 16: frame3=32; break;
	case 32: frame3=64; break;
	case 64: frame3=1; break;
	}
	if (up == 1)
		up = 2; // was 2
	else
		up = 1;
	lc++;
	if (lc == 4) {
		offset++;
                offset1++;
		if (offset == 13) {
			offset = -12;
                        ro++;
                        if (ro == 3) {
                          a = a == 7 ? 1: a+1;
                          ro = 0;
                        }
                }
		if (offset1 == 13)
                        offset1 = -12;
		lc = 0;
	}
}
