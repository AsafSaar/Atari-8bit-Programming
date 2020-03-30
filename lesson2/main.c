////////////////////////////////////
// main.c 
////////////////////////////////////
#include <atari.h>
#include <string.h>
#include <peekpoke.h>

#define SCREEN_MEM		0x5000
#define DLIST_MEM		0x4000		// aligned to 1K
#define CHARSET_MEM		0x3000		// aligned to 1K

#define STICK_RIGHT		7
#define STICK_LEFT		11
#define STICK_UP   		14
#define STICK_DOWN 		13
#define STICK_NOMOVE 	15

#define MOVING_NONE  	0
#define MOVING_UP	 	1
#define MOVING_DOWN  	2
#define MOVING_LEFT  	3
#define MOVING_RIGHT 	4

unsigned char st;
unsigned char moving_type;
unsigned char xpos;

// 32 bytes
unsigned char antic4_display_list[] = 
{
	DL_BLK8,
	DL_BLK8,
	DL_BLK8,
	DL_LMS(DL_CHR40x8x4),
	0x00,
	0x50,
	DL_CHR40x8x4,
	DL_CHR40x8x4,
	DL_CHR40x8x4,
	DL_CHR40x8x4,
	DL_CHR40x8x4,
	DL_CHR40x8x4,
	DL_CHR40x8x4,
	DL_CHR40x8x4,
	DL_CHR40x8x4,
	DL_CHR40x8x4,
	DL_CHR40x8x4,
	DL_CHR40x8x4,
	DL_CHR40x8x4,
	DL_CHR40x8x4,
	DL_CHR40x8x4,
	DL_CHR40x8x4,
	DL_CHR40x8x4,
	DL_CHR40x8x4,
	DL_CHR40x8x4,
	DL_CHR40x8x4,
	DL_CHR40x8x4,
	DL_CHR40x8x4,
	DL_CHR40x8x4,
	DL_JVB,
	0x00,
	0x40
};

unsigned char sprite_dat[] = 
{
	0x14,0x55,0x28,0x28,0x28,0x55,0x55,0x55
};

void wait_for_vblank(void)
{
    asm("lda $14");
wvb:
    asm("cmp $14");
    asm("beq %g", wvb);
} 

void erase_sprite(void)
{
	POKE(SCREEN_MEM + xpos,0);
}

void update_sprite(void)
{
	if (moving_type == MOVING_UP)
	{
		xpos -= 40;
	}
	else if (moving_type == MOVING_DOWN)
	{
		xpos += 40;
	}
	else if (moving_type == MOVING_RIGHT)
	{
		xpos++;
	}
	else if (moving_type == MOVING_LEFT)
	{
		xpos--;
	}
}

void draw_sprite(void)
{
	POKE(SCREEN_MEM + xpos,33);
}

void handle_input(void)
{
	st = OS.stick0;

	if (st == STICK_RIGHT)
	{
		moving_type = MOVING_RIGHT;
	}
	else if (st == STICK_NOMOVE)
	{
		moving_type = MOVING_NONE;
	}
	else if (st ==  STICK_LEFT)
	{
		moving_type = MOVING_LEFT;
	}
	else if (st == STICK_UP)
	{
		moving_type = MOVING_UP;
	}
	else if (st == STICK_DOWN)
	{
		moving_type = MOVING_DOWN;
	}
}

void main(void)
{
	moving_type = MOVING_NONE;

	xpos = 0;
	memcpy((void*)DLIST_MEM,antic4_display_list, sizeof(antic4_display_list));
	
	OS.sdlst = (void*)DLIST_MEM;

	OS.color1 = 0xA3;       // playerfield 1
	OS.color2 = 0X26;		// playerfield 2
	OS.color3 = 0xE1;		// playerfield 3
	OS.color4 = 0x00;		// background

	memcpy((void*)CHARSET_MEM, (void*)0xE000, 0x400);

	// [CHARSETADDRESS] + 8 * charcode
	memcpy((void*) (CHARSET_MEM + 8 * 33) , sprite_dat, 8);

	OS.chbas = CHARSET_MEM >> 8; // MSB / 256

	POKE(SCREEN_MEM + xpos,33);

	while(1)
	{
		// wait for 1 frame to pass
		wait_for_vblank();

		handle_input();

		erase_sprite();
		update_sprite();
		draw_sprite();
	};
}






















/*

#define DLIST 0x3000
#define SCREEN_MEM	0x4000
#define CHARSET_MEM	0x4800

unsigned char dlist_scr[] =
{
	DL_BLK8,                
	DL_BLK8,                
	DL_BLK8,                
    DL_LMS(DL_CHR40x8x4),
    0x00,
    0x40,                     
    DL_CHR40x8x4,                     
    DL_CHR40x8x4,                     
    DL_CHR40x8x4,                     
    DL_CHR40x8x4,                     
    DL_CHR40x8x4,                     
    DL_CHR40x8x4,                     
    DL_CHR40x8x4,                     
    DL_CHR40x8x4,                     
    DL_CHR40x8x4,                     
    DL_CHR40x8x4,                     
    DL_CHR40x8x4,                     
    DL_CHR40x8x4,                     
    DL_CHR40x8x4,                     
    DL_CHR40x8x4,                     
    DL_CHR40x8x4,                     
    DL_CHR40x8x4,                     
    DL_CHR40x8x4,                     
    DL_CHR40x8x4,                     
    DL_CHR40x8x4,                     
    DL_CHR40x8x4,                     
    DL_CHR40x8x4,                     
    DL_CHR40x8x4,                     
    DL_CHR40x8x4,                     
    DL_BLK8,                         
    DL_JVB,                 
    0x00,                      
    0x30                       
};
*/
/*
	00 10 10 00 = 
	10 10 10 10 =
	10 01 01 10 = 
	00 10 10 00 = 
	01 01 01 01 = 
	01 10 10 01 = 

*/

/*unsigned char sprite_dat[] = 
{ 0x28, 0xAA, 0x96, 0xAA, 0x28, 0x55, 0x69, 0x55 };


unsigned char xpos;
unsigned char old_xpos;

void erase_sprite(void)
{
	if (xpos != old_xpos)
	{
		POKE(SCREEN_MEM + xpos, 0);
	}
}

void update_sprite(void)
{
	old_xpos = xpos;
	xpos++;
}

void draw_sprite(void)
{
	POKE(SCREEN_MEM + xpos,0x21);
}

void wait_for_vblank(void)
{
    asm("lda $14");
wvb:
    asm("cmp $14");
    asm("beq %g", wvb);
}

void main(void)
{
	xpos = 0;
	old_xpos = 0;

	OS.color1 = 0X06;
	OS.color2 = 0x46;
	OS.color3 = 0x00;
	OS.color4 = 0X00;

    memcpy((void*)DLIST,dlist_scr, sizeof(dlist_scr));
    OS.sdlst = (void*)DLIST;
	
	memcpy( (void*)CHARSET_MEM,(void*)0xE000,0x400);

	OS.chbas = (CHARSET_MEM >> 8);

	// [CHARSETADDRESS] + 8 * charcode

	memcpy((void*) (CHARSET_MEM + 8*33),sprite_dat,8);

	POKE(SCREEN_MEM + xpos,0x21);


	while (1) 
	{
		wait_for_vblank();
		
		erase_sprite();
		update_sprite();
		draw_sprite();
	}
}*/