#include <atari.h>
#include <string.h>
#include <peekpoke.h>

#define CHARSET_MEM0	0x3000		// aligned to 1K
#define DLIST_MEM		0x4000		// aligned to 1K
#define SCREEN_MEM		0x5000
#define PMG_MEM			0x6000		// aligned to 2K

#define STICK_DOWN 		13
#define TRIG_FIRE		0

#define STAND 		 	0
#define RUN	 			1
#define DUCK	  		2
#define JUMP 			3

#define SCREEN_WIDTH 	40
#define SCREEN_HIGHT	24

#define FRAME_SPEED		4
#define FLOOR 			0x70
#define JUMP_TOP		FLOOR - 0x45
#define JUMP_SPEED 		7
#define FLY_GRACE 		8
#define ENEMY_SPEED		3
#define BIRD_INTERVAL 	30

#define CACTUS1_CHAR 	60 // up to 65

unsigned char frame_count;
unsigned char frame;
unsigned char bird_frame;
unsigned char stick;
unsigned char trigger;
unsigned char moving_type = STAND;
unsigned int player_ypos;
unsigned char bird_ypos;
unsigned char bird_xpos;
unsigned char summon_bird;
unsigned char game_over = 1;
unsigned char vblank_occured = 0;
unsigned char * banner = "SCORE:      HIGH-SCORE:";
unsigned char * press_start = "  PRESS START (F2) TO START RUNNING";
unsigned char banner_len;
unsigned char start_len;
unsigned char is_jumping;
unsigned char is_falling;
unsigned char gravity = 4;
unsigned int cactus1_xpos;
unsigned char fly_grace;
unsigned char collision_detected = 0;
unsigned int score;
unsigned int hi_score;
unsigned char score_banner[3];
unsigned char hi_score_banner[3];

// 32 bytes
unsigned char antic4_display_list[] = 
{
	DL_BLK8,
	DL_BLK8,
	DL_BLK8,
	DL_LMS(DL_CHR40x8x1),
	0x00,
	0x50,
	DL_CHR40x8x1,
	DL_CHR40x8x4,
	DL_CHR40x8x4,
	DL_CHR40x8x4,
	DL_CHR40x8x4,
	DL_CHR40x8x4,
	DL_CHR40x8x4,
	DL_CHR40x8x4,
	DL_CHR40x8x4,
	DL_CHR40x8x4,
	DL_DLI(DL_CHR40x8x4),
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

#define FRAMES (6)
#define HEIGHT (22)

const unsigned char P0DATA[FRAMES][HEIGHT] =
{
  {
    0x01, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x83, 
    0x83, 0xc7, 0xcf, 0xff, 0xff, 0xff, 0x7f, 0x3f, 
    0x1f, 0x1b, 0x19, 0x11, 0x11, 0x19
  },
  {
    0x01, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x83, 
    0x83, 0xc7, 0xcf, 0xff, 0xff, 0xff, 0x7f, 0x3f, 
    0x1f, 0x1b, 0x19, 0x10, 0x10, 0x18
  },
  {
    0x01, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x83, 
    0x83, 0xc7, 0xcf, 0xff, 0xff, 0xff, 0x7f, 0x3f, 
    0x1f, 0x13, 0x19, 0x01, 0x01, 0x01
  },
  {
    0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0xe0, 
    0xe7, 0xff, 0xff, 0xff, 0x7f, 0x7f, 0x7f, 0x7f, 
    0x5f, 0x4f, 0x6e, 0x0c, 0x08, 0x0c
  },
  {
    0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0xe0, 
    0xe7, 0xff, 0xff, 0xff, 0x7f, 0x7f, 0x7f, 0x7f, 
    0x7f, 0x6f, 0x6e, 0x48, 0x4c, 0x60
  },
  {
    0x01, 0x02, 0x02, 0x02, 0x03, 0x03, 0x03, 0x83, 
    0x83, 0xc7, 0xcf, 0xff, 0xff, 0xff, 0x7f, 0x3f, 
    0x1f, 0x1b, 0x19, 0x11, 0x11, 0x19
  }
};
const unsigned char P1DATA[FRAMES][HEIGHT] =
{
  {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  },
  {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  },
  {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  },
  {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  },
  {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  },
  {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  }
};

const unsigned char P2DATA[FRAMES][HEIGHT] =
{
  {
    0xf0, 0xf8, 0x78, 0xf8, 0xf8, 0xf8, 0xf8, 0x80, 
    0xf0, 0x80, 0xf0, 0xd0, 0xc0, 0xc0, 0x80, 0x80, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x80
  },
  {
    0xf0, 0xf8, 0x78, 0xf8, 0xf8, 0xf8, 0xf8, 0x80, 
    0xf0, 0x80, 0xf0, 0xd0, 0xc0, 0xc0, 0x80, 0x80, 
    0x00, 0x00, 0x80, 0x00, 0x00, 0x00
  },
  {
    0xf0, 0xf8, 0x78, 0xf8, 0xf8, 0xf8, 0xf8, 0x80, 
    0xf0, 0x80, 0xf0, 0xd0, 0xc0, 0xc0, 0x80, 0x80, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x80
  },
  {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x3e, 0xff, 0xff, 0xef, 0xff, 0xff, 0xff, 0xf8, 
    0xbe, 0x80, 0x80, 0xc0, 0x00, 0x00
  },
  {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x3e, 0xff, 0xff, 0xef, 0xff, 0xff, 0xff, 0xf8, 
    0xbe, 0x80, 0x80, 0xc0, 0x00, 0x00
  },
  {
    0xf0, 0x38, 0xb8, 0x38, 0xf8, 0xf8, 0xf8, 0xf0, 
    0xf0, 0x80, 0xf0, 0xd0, 0xc0, 0xc0, 0x80, 0x80, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x80
  }
};
const unsigned char P3DATA[FRAMES][HEIGHT] =
{
  {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  },
  {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  },
  {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  },
   {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  },
  {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  },
  {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  }
};

const unsigned char cactus1_data[6][8] =
{
	{0x00, 0x00, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41},
	{0x41, 0x41, 0x41, 0x55, 0x55, 0x15, 0x05, 0x05},
	{0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01},
	{0x00, 0x40, 0x50, 0x50, 0x50, 0x51, 0x51, 0x51},
	{0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51},
	{0x55, 0x54, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50}
};
const unsigned char M0DATA[2][16] =
{
  {
    0x08, 0x08, 0x28, 0x6c, 0xec, 0xec, 0x2e, 0x3f, 
    0x3e, 0x3f, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00
  },
  {
    0x00, 0x00, 0x20, 0x60, 0xe0, 0xe0, 0x38, 0x3f, 
    0x3e, 0x3f, 0x3e, 0x38, 0x38, 0x30, 0x30, 0x20
  }
};
void dli_routine0(void);
// void dli_routine1(void);
// void dli_routine2(void);
// void dli_routine3(void);
void erase_sprite(void);
void update_bird_sprite(void);

unsigned char atascii_to_internal(char * str) // converts atascii string to internal and returns string length 
{
	unsigned char i = 0;
	do {
		str[i] -= 32;
		i++;
	}while (str[i] != 0);
	return i;
}

void restart_game(void)
{
	antic4_display_list[6] = DL_CHR40x8x4;
	memset((void*)(SCREEN_MEM + SCREEN_WIDTH), 0, start_len);
	memcpy((void*)DLIST_MEM,antic4_display_list, sizeof(antic4_display_list));
	erase_sprite();
	cactus1_xpos = 11 * SCREEN_WIDTH -2;
	game_over = 0;
	bird_xpos = 20;
	update_bird_sprite();
	score = 0;
	summon_bird = 0;
	GTIA_WRITE.hitclr = 1;  // clear p/m collision
	memcpy((void*)SCREEN_MEM, (void*)banner, banner_len); // draw static banner 
}


void wait_for_vblank(void)
{
	while (vblank_occured == 0)
	{
	};

	vblank_occured = 0;
}

void wait_for_vblank_clock(void)
{
    asm("lda $14");
	wvb:
    asm("cmp $14");
    asm("beq %g", wvb);
}

void erase_sprite(void)
{
	memset((void*)(PMG_MEM + 0x400 + player_ypos),0,22);
    memset((void*)(PMG_MEM + 0x500 + player_ypos),0,22);
    memset((void*)(PMG_MEM + 0x600 + player_ypos),0,22);
    memset((void*)(PMG_MEM + 0x700 + player_ypos),0,22);

	POKE(SCREEN_MEM + cactus1_xpos,0);
	POKE(SCREEN_MEM + cactus1_xpos + SCREEN_WIDTH,0);
	POKE(SCREEN_MEM + cactus1_xpos + 2 * SCREEN_WIDTH,0);
	POKE(SCREEN_MEM + cactus1_xpos + 1,0);
	POKE(SCREEN_MEM + cactus1_xpos + 1 + SCREEN_WIDTH,0);
	POKE(SCREEN_MEM + cactus1_xpos + 1 + 2 * SCREEN_WIDTH,0);

}


void update_player_sprite(void)
{
	if (is_jumping == 1)
	{
		player_ypos -= JUMP_SPEED;
		if (player_ypos <= JUMP_TOP)
		{
			is_jumping = 0;
			is_falling = 1;
		}
	}
	
	if (is_falling == 1) 
	{
		if (fly_grace > 0) fly_grace--; 
	}
	
	if (fly_grace <= 0) 
	{
		player_ypos += gravity;
	}

	if (player_ypos >= FLOOR)
	{
		player_ypos = FLOOR;
		is_falling = 0;
		fly_grace = FLY_GRACE;
	}

		// draw sprites
	if (frame_count % FRAME_SPEED == 0) // update animation every FRAME_SPEED
	{
		bird_frame = !bird_frame;
		if (game_over)
		{
			frame = 5;
		}
		 else if (moving_type == STAND)
		{
			frame = 0;
		}
		else if (moving_type == RUN)
		{
			frame = frame%2 + 1;
		} 
		else if (moving_type == JUMP)
		{
			frame = 0;
		}
		else if (moving_type == DUCK)
		{
			frame = frame%2 + 3;
		}
	}
}

void update_cactus_sprite(void)
{
	if (frame_count % ENEMY_SPEED == 0 && !game_over) 
		cactus1_xpos--;

	if (cactus1_xpos < 10 * SCREEN_WIDTH) // loop animation when reaching end of screen
	{
		cactus1_xpos = 11 * SCREEN_WIDTH -2;
	}	
}
void update_bird_sprite(void)
{
	if (!game_over && summon_bird)
	{
		bird_xpos--;
		bird_xpos--;
	}
	if (bird_xpos < 40)
	{
		summon_bird = 0;
		bird_xpos = 20;
	}
}

void update_score(void)
{
	if (frame_count % (ENEMY_SPEED + 10) == 0 && !game_over) // calculate score only once in ENEMY_SPEED + constant
	{
		score++;
		score_banner[0] = 16 + (score % 10);
		score_banner[1] = 16 + (score / 10) % 10;
		score_banner[2] = 16 + (score / 100) % 10;
		if (score % BIRD_INTERVAL == 0)
		{
			summon_bird = 1;
			bird_xpos = 200;
		}
	}

}

void update_sprite(void)
{
	update_player_sprite();
	update_cactus_sprite();
	update_bird_sprite();
	update_score();
}


void draw_sprite(void)
{
	// draw score
	POKE(SCREEN_MEM + 9, score_banner[0]);	// 1
	POKE(SCREEN_MEM + 8, score_banner[1]);	// 10
	POKE(SCREEN_MEM + 7, score_banner[2]);	// 100

	POKE(SCREEN_MEM + 26, hi_score_banner[0]);	// 1
	POKE(SCREEN_MEM + 25, hi_score_banner[1]);	// 10
	POKE(SCREEN_MEM + 24, hi_score_banner[2]);	// 100
	
	// draw charachters
	POKE(SCREEN_MEM + cactus1_xpos,CACTUS1_CHAR);
	POKE(SCREEN_MEM + cactus1_xpos + SCREEN_WIDTH,CACTUS1_CHAR+1);
	POKE(SCREEN_MEM + cactus1_xpos + 2 * SCREEN_WIDTH,CACTUS1_CHAR+2);
	POKE(SCREEN_MEM + cactus1_xpos + 1,CACTUS1_CHAR+3);
	POKE(SCREEN_MEM + cactus1_xpos + 1 + SCREEN_WIDTH,CACTUS1_CHAR+4);
	POKE(SCREEN_MEM + cactus1_xpos + 1 + 2 * SCREEN_WIDTH,CACTUS1_CHAR+5);
	
    memcpy((void*)(PMG_MEM + 0x300 + bird_ypos),&M0DATA[bird_frame],16);
    memcpy((void*)(PMG_MEM + 0x400 + player_ypos),&P0DATA[frame],22);
    memcpy((void*)(PMG_MEM + 0x500 + player_ypos),&P1DATA[frame],22);
    memcpy((void*)(PMG_MEM + 0x600 + player_ypos),&P2DATA[frame],22);
    memcpy((void*)(PMG_MEM + 0x700 + player_ypos),&P3DATA[frame],22);


	GTIA_WRITE.hposm0 = bird_xpos;
    GTIA_WRITE.hposm1 = bird_xpos - 2;
	GTIA_WRITE.hposm2 = bird_xpos - 4;
	GTIA_WRITE.hposm3 = bird_xpos - 6;
}

void handle_input(void)
{
	stick = OS.stick0;
	trigger = OS.strig0;
	
	if (!game_over)
	{
		if (stick == STICK_DOWN)
		{
			moving_type = DUCK;
		}
		else if (trigger == TRIG_FIRE)
		{
			if (is_jumping != 1 && is_falling != 1)
			{
				is_jumping = 1;
				moving_type = JUMP;
				fly_grace = FLY_GRACE;
			}
		}
		else 
		{
			moving_type = RUN;
		}	
	}
	
	if (game_over == 1)
	{
		if (CONSOL_START(GTIA_READ.consol))
		{
			restart_game();
		}		
	}
}

void detect_collision(void)
{
	collision_detected = (GTIA_READ.p0pf || GTIA_READ.m0pl || GTIA_READ.m1pl || GTIA_READ.m2pl || GTIA_READ.m3pl);
}

void vbi_routine(void)
{
	vblank_occured = 1;
	frame_count++;
	asm("jmp $E45F"); // continue normal vblank flow (SYSVBV)
}

void dli_routine0(void) // handle sky and ufo charset
{
	asm("pha");
    asm("txa");
    asm("pha");
    asm("tya");
    asm("pha");

	ANTIC.wsync = 1;
	ANTIC.wsync = 1;
	ANTIC.wsync = 1;
	GTIA_WRITE.colbk = 0x10;
	ANTIC.wsync = 1;
	GTIA_WRITE.colbk = 0x12;
	ANTIC.wsync = 1;
	GTIA_WRITE.colbk = 0x14;
	ANTIC.wsync = 1;
	GTIA_WRITE.colbk = 0x18;
	
    asm("pla");
    asm("tay");
    asm("pla");
    asm("tax");
    asm("pla");
    asm("rti");
	// OS.vdslst = &dli_routine1;
}

// void dli_routine1(void) // handle player charset
// {
// 	asm("pha");
//     asm("txa");
//     asm("pha");
//     asm("tya");
//     asm("pha");
// 	ANTIC.wsync = 1;

//     asm("pla");
//     asm("tay");
//     asm("pla");
//     asm("tax");
//     asm("pla");
//     asm("rti");
// 	OS.vdslst = &dli_routine2;
// }

// void dli_routine2(void) // handle game over banner colors
// {
// 	asm("pha");
//     asm("txa");
//     asm("pha");
//     asm("tya");
//     asm("pha");

// 	ANTIC.wsync = 1;

//     asm("pla");
//     asm("tay");
//     asm("pla");
//     asm("tax");
//     asm("pla");
//     asm("rti");
// 	OS.vdslst = &dli_routine3;
// }

// void dli_routine3(void) // paint ground
// {
// 	asm("pha");
//     asm("txa");
//     asm("pha");
//     asm("tya");
//     asm("pha");

// 	ANTIC.wsync = 1;

//     asm("pla");
//     asm("tay");
//     asm("pla");
//     asm("tax");
//     asm("pla");
//     asm("rti");
// 	OS.vdslst = &dli_routine0;
// }
void init_nmis(void)
{
	wait_for_vblank_clock();
	OS.vvblki = &vbi_routine;
	OS.vdslst = &dli_routine0;
	ANTIC.nmien = NMIEN_DLI | NMIEN_VBI;

}

void handle_game_over(void)
{
	if (collision_detected != 0 || score >= 300)
		{
			game_over = 1;
			if (score > hi_score)
			{
				hi_score = score;
				hi_score_banner[0] = score_banner[0];
				hi_score_banner[1] = score_banner[1];
				hi_score_banner[2] = score_banner[2];
			}
		}
}

void setup_pmg(void)
{	
	// tell ANTIC what is the memory address of the PMG memory
    ANTIC.pmbase = PMG_MEM >> 8;

    // clear PMG memory
	bzero((void*)(PMG_MEM+0x300),0x500);


	// tell GTIA what type of PMG do we want?
	GTIA_WRITE.gractl = GRACTL_MISSLES + GRACTL_PLAYERS;

    // set the priority register (order of objects appear on screen)
	// PRIOR_P03_PF03 | PRIOR_5TH_PLAYER | PRIOR_OVERLAP_3RD_COLOR
    OS.gprior = PRIOR_P03_PF03 | PRIOR_OVERLAP_3RD_COLOR | PRIOR_5TH_PLAYER;

    // player colors , set shadows
    OS.pcolr0 = 0x00;
    OS.pcolr1 = 0x00;
    OS.pcolr2 = 0x00;
    OS.pcolr3 = 0x00;

    // missiles combining the 5th player get their color from location 711
    // which is playfield 3
	OS.color3 = 0x26; 

    // player size (normal, double, quad)
    GTIA_WRITE.sizem  = PMG_SIZE_NORMAL;	
    GTIA_WRITE.sizep0 = PMG_SIZE_NORMAL;
    GTIA_WRITE.sizep1 = PMG_SIZE_NORMAL;
    GTIA_WRITE.sizep2 = PMG_SIZE_NORMAL;
    GTIA_WRITE.sizep3 = PMG_SIZE_NORMAL;

	player_ypos = 0x50;
    bird_ypos = 0x65;
    bird_xpos = 0x00;
    
    // missile horizontal position
    GTIA_WRITE.hposm0 = bird_xpos;
    GTIA_WRITE.hposm1 = bird_xpos -2;
    GTIA_WRITE.hposm2 = bird_xpos -4;
    GTIA_WRITE.hposm3 = bird_xpos -6;

    // player horizontal position
    GTIA_WRITE.hposp0 = 0x41;
//    GTIA_WRITE.hposp1 = 0x41;
    GTIA_WRITE.hposp2 = 0x49;
//    GTIA_WRITE.hposp3 = 0x49;

    // memset((void*)(PMG_MEM + 0x400 + 0x90),255,8);
}

void main(void)
{
	// shut down ANTIC DMA
	OS.sdmctl = 0;

	// set custom display liat
	memcpy((void*)DLIST_MEM,antic4_display_list, sizeof(antic4_display_list));
	OS.sdlst = (void*)DLIST_MEM;
	
	// init colors
	OS.color0 = 0x00;       // playerfield 0
	OS.color1 = 0XFF;		// playerfield 1
	OS.color2 = 0X00;		// playerfield 2
	OS.color3 = 0X00;		// playerfield 3
	OS.color4 = 0xFF;		// background

	// copy original charcter set to new location
	memcpy((void*)CHARSET_MEM0, (void*)0xE000, 0x400);
	// override original characters with custom ones
	memcpy((void*) (CHARSET_MEM0 + 8 * CACTUS1_CHAR) , cactus1_data[0], 8);
	memcpy((void*) (CHARSET_MEM0 + 8 * (CACTUS1_CHAR+1)) , cactus1_data[1], 8);
	memcpy((void*) (CHARSET_MEM0 + 8 * (CACTUS1_CHAR+2)) , cactus1_data[2], 8);
	memcpy((void*) (CHARSET_MEM0 + 8 * (CACTUS1_CHAR+3)) , cactus1_data[3], 8);
	memcpy((void*) (CHARSET_MEM0 + 8 * (CACTUS1_CHAR+4)) , cactus1_data[4], 8);
	memcpy((void*) (CHARSET_MEM0 + 8 * (CACTUS1_CHAR+5)) , cactus1_data[5], 8);

	OS.chbas = CHARSET_MEM0 >> 8; // MSB / 256

	banner_len = atascii_to_internal(banner);
	start_len = atascii_to_internal(press_start);

	init_nmis();
	setup_pmg();
	memcpy((void*)(SCREEN_MEM + SCREEN_WIDTH), (void*)press_start, start_len);

    // tell ANTIC do we work with single line or double line resolution 
    OS.sdmctl = 0x3E; // single line

	while(1)
	{
		// wait for 1 frame to pass
		wait_for_vblank();
		handle_input();
		erase_sprite();
		update_sprite();
		draw_sprite();
		detect_collision();
		handle_game_over();
	};
}