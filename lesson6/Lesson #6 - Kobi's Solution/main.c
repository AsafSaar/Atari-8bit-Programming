/************************************************
** Image to Antic CC65 code		       **
** Generated with image2antic.py               **
** https://github.com/tyrkelko/image2antic     **
************************************************/
#include <atari.h>
#include <string.h>
#include <peekpoke.h>

#define CHARSET0_MEM	0x5000
#define SCREEN_MEM		0x7000
#define DLIST_MEM		0x6C00	// aligned to 1K
#define SPRITES_MEM		0x6D00

// Strings reference
#define KEY_ANIM_F0					0
#define KEY_ANIM_F1        			1
#define KEY_ANIM_F2                	2
#define SNAKE_ANIM_LF0              3
#define SNAKE_ANIM_LF1              4
#define SNAKE_ANIM_LF2              5
#define SNAKE_ANIM_LF3              6
#define SNAKE_ANIM_RF0           	7
#define SNAKE_ANIM_RF1              8
#define SNAKE_ANIM_RF2              9
#define SNAKE_ANIM_RF3              10
#define COIN_ANIM_F0 	            11
#define COIN_ANIM_F1 	            12
#define COIN_ANIM_F2 	            13
#define COIN_ANIM_F3 	            14
#define COIN_ANIM_F4 	            15
#define COIN_ANIM_F5 	            16

#define SPRITE_NUM					17

//extern unsigned char *L_DLIST_MEM;
extern unsigned char *L_SCREEN_MEM;
extern unsigned char *L_CHARSET0_MEM;
extern unsigned char *L_CHARSET1_MEM;
extern unsigned char *L_SPRITES_MEM;

unsigned char sprites[] = {	0,	2,	4,  	6,	10,	14,	18, 	22,	26,	30,	34, 	38,	40,	42,	44,	46,	48,		50	}; // end of last sprite frame

unsigned char key_animation_sequence[] = {KEY_ANIM_F0, KEY_ANIM_F1, KEY_ANIM_F2, KEY_ANIM_F1};
unsigned char snake_animation_left_sequence[] = {SNAKE_ANIM_LF3, SNAKE_ANIM_LF2, SNAKE_ANIM_LF1, SNAKE_ANIM_LF0};
unsigned char snake_animation_right_sequence[] = {SNAKE_ANIM_RF0, SNAKE_ANIM_RF1, SNAKE_ANIM_RF2, SNAKE_ANIM_RF3};
unsigned char coin_animation_sequence[] = {COIN_ANIM_F0, COIN_ANIM_F1, COIN_ANIM_F2, COIN_ANIM_F3, COIN_ANIM_F4, COIN_ANIM_F5};

unsigned char i,j;

unsigned char snake_x_pos[] = {20, 20, 20};
unsigned char snake_y_pos[] = {2, 6, 10};
unsigned char snake_dir[] = {1,1,1};
unsigned char snake_left_bound[] = {16, 19, 8};
unsigned char snake_right_bound[] = {21, 34, 30};
unsigned char snake_frame[] = {SNAKE_ANIM_LF0,SNAKE_ANIM_LF0,SNAKE_ANIM_LF0};

unsigned char coin_x_pos[] = {19, 22, 25, 14, 17, 20};
unsigned char coin_y_pos[] = {4, 4, 4, 8, 8, 8};
unsigned char coin_frame[] = {0, 1, 2, 3, 4, 5};

unsigned char key_x_pos = 9;
unsigned char key_y_pos = 8;
unsigned char key_frame = 0;

unsigned char vblank_counter;

unsigned char charset_array[] = {CHARSET0_MEM >> 8};

// CHARSET DLI CHANGES IN LINES: [0]
unsigned char antic4_display_list[] = {
	DL_BLK8,
	DL_BLK8,
	DL_DLI(DL_BLK8),
	DL_LMS(DL_CHR40x16x4),
	0x00,
	SCREEN_MEM >> 8,
	DL_CHR40x16x4,
	DL_CHR40x16x4,
	DL_CHR40x16x4,
	DL_CHR40x16x4,
	DL_CHR40x16x4,
	DL_CHR40x16x4,
	DL_CHR40x16x4,
	DL_CHR40x16x4,
	DL_CHR40x16x4,
	DL_CHR40x16x4,
	DL_CHR40x16x4,
	DL_JVB,
	0x00,
};


unsigned char charset_index = 0;
unsigned char charset_array_size;

void erase_sprite(void);
void update_sprite(void);
void draw_sprite(void);
void handle_input (void);
void wait_for_vblank(void);
void setup_dli(void);
void dli_routine_0(void);
void dli_routine_1(void);

void main(void)
{
	// CREATE SCREEN
	memcpy((void*) DLIST_MEM,antic4_display_list,sizeof(antic4_display_list));
	OS.sdlst=(void*)DLIST_MEM;

	// SET COLORS
	OS.color0 = 250;
	OS.color1 = 252;
	OS.color2 = 242;
	OS.color3 = 134;
	OS.color4 = 0;
	
	charset_array_size = sizeof(charset_array);
	setup_dli();
	wait_for_vblank();
	ANTIC.nmien = NMIEN_DLI | NMIEN_VBI; 

	OS.sdmctl = 0x3E;
	// GAME LOOP
	while (1) {
		wait_for_vblank();
		erase_sprite();
		handle_input();
		update_sprite();
		draw_sprite();
	};
}

void wait_for_vblank(void)
{
    asm("lda $14");
wvb:
    asm("cmp $14");
    asm("beq %g", wvb);
    ++vblank_counter;
} 
void dli_routine_0(void)
{
    asm("pha");
    asm("tya");
    asm("pha");
    asm("tya");
    asm("pha");
    ANTIC.chbase = charset_array[charset_index];
    ANTIC.wsync = 1;
    charset_index++;
    if (charset_index >= charset_array_size) charset_index = 0;
    asm("pla");
    asm("tay");
    asm("pla");
    asm("tax");
    asm("pla");
    asm("rti");
}

void setup_dli(void)
{
	wait_for_vblank();
    OS.vdslst = &dli_routine_0;
}
void erase_sprite() 
{
	// ERASE SPRITE
}
void update_sprite() 
{
	// Update key sprite frames	
	if (((vblank_counter&15)==0) && ((++key_frame)>3)) key_frame=0;
	i = 3;
	while(i > 0)
	{
		--i;
		if (((vblank_counter&3)==0) && ((++snake_frame[i])>3)) 
			{
				snake_frame[i] = 0;
				if (snake_dir[i] == 1) 
				{
					snake_x_pos[i]--;
					if (snake_x_pos[i] <= snake_left_bound[i])
					{
						snake_dir[i] = 2;
					}
				}
				else if (snake_dir[i] == 2)
				{
					snake_x_pos[i]++;
					if (snake_x_pos[i] >= snake_right_bound[i])
					{
						snake_dir[i] = 1;
					}
				}
			}

	}
	i = 6;
	while(i > 0)
	{
		--i;
		if (((vblank_counter&7)==0) && ((++coin_frame[i]) > 4))
		{
			coin_frame[i] = 0;
		}
	}
}
void draw_sprite() 
{
	// DRAW KEY SPRITE
	memcpy((void*)(SCREEN_MEM + key_y_pos*40+key_x_pos), 
		(void*)(SPRITES_MEM + sprites[key_animation_sequence[key_frame]]), 
		sprites[key_animation_sequence[key_frame] + 1] - sprites[key_animation_sequence[key_frame]]
	);

	// DRAW SNAKES SPRITES
	i = 3;
	while (i > 0)
	{
		--i;
		if (snake_dir[i] == 2)
		{
			j = snake_animation_left_sequence[snake_frame[i]];
		}
		else
		{
			j = snake_animation_right_sequence[snake_frame[i]];	
		}
		memcpy((void*)(SCREEN_MEM + snake_y_pos[i]*40+snake_x_pos[i]), 
			(void*)(SPRITES_MEM + sprites[j]), 
			4
		);

	}

	// DRAW COIN SPRITES
	i = 6;
	while (i > 0)
	{
		--i;
		memcpy((void*)(SCREEN_MEM + coin_y_pos[i]*40+coin_x_pos[i]), 
			(void*)(SPRITES_MEM + sprites[coin_animation_sequence[coin_frame[i]]]), 
			2
		);
	}

}
void handle_input (void)
{
 // TODO: HANDLE INPUT
}
