////////////////////////////////////
// main.c 
////////////////////////////////////
#include <atari.h>
#include <peekpoke.h>
#include <string.h>

#define DISPLAY_LIST_MEM	0x3000
#define SCREEN_MEM			0x4100
#define SCREEN_MEM_BOTTOM   0x5000


unsigned char DisplayList[] = 
{
	DL_BLK8, // 0
	DL_BLK8, // 1
	DL_BLK8, // 2
	DL_LMS(DL_MAP160x1x4), // 3
	0, // 4
	0, // 5
	// 6
	DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,
	// 13
	DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,
	DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,
	DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,
	DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,
	DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,
	DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,
	DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,
	DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,
	DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,
	DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,
	DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,
	// 101
	DL_LMS(DL_MAP160x1x4),
	0, // 102
	0, // 103
	DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,
	// 110
	DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,
	DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,
	DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,
	DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,
	DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,
	DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,
	DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,
	DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,
	DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,
	DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,
	DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,DL_MAP160x1x4,
	// 198
	DL_JVB,
	0, // 199
	0 // 200
};

#define JOY_BIT_UP    1
#define JOY_BIT_DOWN  2
#define JOY_BIT_LEFT  4
#define JOY_BIT_RIGHT 8

#define HEIGHT (6)
unsigned char spriteShifted[8][HEIGHT] = 
{ 
	{0,60,255,255,60,0},		// original left byte
	{0,0,0,0,0,0},				// right byte 0

	{0,0,0,0,0,0},				// left byte shift >> 2
	{0,0,0,0,0,0},				// right byte shift << 6

	{0,0,0,0,0,0},				// left byte shift >> 4
	{0,0,0,0,0,0},				// right byte shift << 4

	{0,0,0,0,0,0},				// left byte shift >> 6
	{0,0,0,0,0,0},				// right byte shift << 2

};
unsigned char preVal = 0;
unsigned char i = 0, j=0,q=0,st=0;
unsigned int old_screen_address = 0;
unsigned int screen_address = 0;
unsigned char ballMoved = 1;
unsigned char currClockFrame = 0;
unsigned char first_run = 1;

int t = 0,oldJ = 255, oldQ = 255;
unsigned int screen_row_address = SCREEN_MEM;
unsigned int old_screen_row_address = SCREEN_MEM;


void waitForVBLANK(void)
{
    currClockFrame = OS.rtclok[2];
    while (OS.rtclok[2] == currClockFrame);
}

void setupDisplayList(void)
{	
	DisplayList[4] = SCREEN_MEM%256;
	DisplayList[5] = SCREEN_MEM>>8;
	DisplayList[102] = SCREEN_MEM_BOTTOM %256;
	DisplayList[103] = SCREEN_MEM_BOTTOM >>8;
	
	DisplayList[199] = DISPLAY_LIST_MEM%256;
	DisplayList[200] = DISPLAY_LIST_MEM>>8;
	memcpy((void*)DISPLAY_LIST_MEM,&DisplayList,sizeof(DisplayList));
	OS.sdlst = (void*)DISPLAY_LIST_MEM;
	bzero((void*)SCREEN_MEM,7680);
}

void createShiftedFrames()
{
	for (i=0;i<HEIGHT;i++)
	{
		spriteShifted[2][i] = spriteShifted[0][i] >> 2; // left byte shift >> 2
		spriteShifted[3][i] = spriteShifted[0][i] << 6; // right byte shift << 6

		spriteShifted[4][i] = spriteShifted[0][i] >> 4; // left byte shift >> 4
		spriteShifted[5][i] = spriteShifted[0][i] << 4; // right byte shift << 4

		spriteShifted[6][i] = spriteShifted[0][i] >> 6; // left byte shift >> 6
		spriteShifted[7][i] = spriteShifted[0][i] << 2; // right byte shift << 2
	}
}

void handleJoystick(void)
{
	ballMoved = 0;
	st = (OS.stick0 ^ 0x0f);
	if (st & JOY_BIT_RIGHT)
	{
		ballMoved = 1;
		j+=2;
		if (j>7)
		{
			q+=1;
			j=0;
		}

	};

	if (st & JOY_BIT_LEFT)
	{
		ballMoved = 1;
		j-=2;
		if (j==254)
		{
			q-=1;
			j=6;
		}
	}

	if (st & JOY_BIT_DOWN)
	{
		ballMoved = 1;
		old_screen_row_address += 40;
	}

	if (st & JOY_BIT_UP)
	{
		ballMoved = 1;
		screen_row_address -= 40;
	}
}

void updateBall(void)
{
	// check if the ball is not moving
	if (ballMoved || first_run)
	{
		first_run = 0;
		screen_address = screen_row_address + q;
		old_screen_address = old_screen_row_address + oldQ;

		for (i=0;i<HEIGHT;i++)
		{
			POKE(old_screen_address,0);
			old_screen_address+=1;
			POKE(old_screen_address,0);
			old_screen_address+=39;
		}

		for (i=0;i<HEIGHT;i++)
		{
			POKE(screen_address,spriteShifted[j][i]);
			screen_address += 1;
			POKE(screen_address,spriteShifted[j+1][i]);
			screen_address += 39;
		}

		oldJ = j;
		oldQ = q;
		old_screen_row_address = screen_row_address;
	}	
}

int main()
{
	// turn off ANTIC
	preVal = OS.sdmctl;
	OS.sdmctl = 0;

	setupDisplayList();
	createShiftedFrames();

	// turn ANTIC back on
	OS.sdmctl = preVal;

	q=0;
	j=0;
	while (1)
	{
		// first wait for screen frame
		waitForVBLANK(); 
		handleJoystick();
		updateBall();	
	}

	return 0;
}