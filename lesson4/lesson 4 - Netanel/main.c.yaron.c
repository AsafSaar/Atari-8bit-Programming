////////////////////////////////////
// main.c 
////////////////////////////////////
#include <atari.h>
#include <string.h>
#include <peekpoke.h>

//code - 2000
//PMG  - 4000 + 0x800 // 2k
//DLIST_MEM     - 4800 + 32 bytes
// SCREEN_MEM   - 4900 - ANTIC mode 4 960 byte 
// CHARSET_MEM  - 5000 - 1K 0x400 (5400) 

#define PMG_MEM		0x6000		// aligned to 2K
#define SCREEN_MEM	0x5000
#define DLIST_MEM	0x4000		// aligned to 1K
#define CHARSET_MEM	 0x3000		// aligned to 1K
#define CHARSET_MEM2 0x3400     // aligned to 1K

#define JOY_NO_MOVE   0
#define JOY_BIT_UP    1
#define JOY_BIT_DOWN  2
#define JOY_BIT_LEFT  4
#define JOY_BIT_RIGHT 8
#define JOY_BIT_TOP_RIGHT   9
#define JOY_BIT_TOP_LEFT    5

#define MOVING_NONE  0
#define MOVING_UP	 1
#define MOVING_DOWN  2
#define MOVING_LEFT  3
#define MOVING_RIGHT 4

#define P0PF 0xD004


unsigned char st;
unsigned char vblank_occured;
unsigned char moving_type;

unsigned char player0_data[] =
{
    0x38, 0x38, 0x38, 0x10, 0x00, 0x00, 0x38, 0x38, 
    0x38, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x08, 0x64, 0x44, 0xc6, 0x00, 0x00, 0x00
};

unsigned char player1_data[] = 
{
	0x00, 0x00, 0x00, 0x00, 0xb8, 0xfd, 0xff, 0xfc,
    0x7c, 0x7c, 0x38, 0x38, 0x38, 0x28, 0x28, 0x28,
    0x28, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

unsigned char missiles_data[] = 
{
	0x3c, 0x18, 0x18, 0x38, 0x7c, 0x7c, 0x7e, 0xfd, 
    0xbd, 0xbd, 0xbd, 0x3c, 0x18, 0x18, 0x18, 0x18, 
    0x3c, 0x00, 0x00
};

unsigned char player_xpos;
unsigned char player_ypos;

// 32 bytes
unsigned char antic4_display_list[] = 
{
	DL_BLK8,
	DL_BLK8,
	DL_DLI(DL_BLK8),			// routine 
	DL_LMS(DL_CHR40x8x4),
	0x00,
	0x50,
	DL_CHR40x8x4,
	DL_CHR40x8x4,
	DL_DLI(DL_CHR40x8x4),		// routine 1
	DL_CHR40x8x4,
	DL_CHR40x8x4,
	DL_CHR40x8x4,
	DL_CHR40x8x4,
	DL_CHR40x8x4,
	DL_CHR40x8x4,
	DL_CHR40x8x4,
	DL_CHR40x8x4,
	DL_CHR40x8x4,
	DL_DLI(DL_CHR40x8x4),		// routine 2
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

//00 - background
//01 - color1
//10 - color 2
//11 - color 3

// 00 01 01 00
// 01 01 01 01
// 00 10 10 00
// 00 10 10 00
// 00 10 10 00
// 01 01 01 01
// 01 01 01 01
// 01 01 01 01



unsigned char sprite_dat[] = 
{
	0x14,0x55,0x28,0x28,0x28,0x55,0x55,0x55
};

void wait_for_vblank_clock(void)
{
    asm("lda $14");
wvb:
    asm("cmp $14");
    asm("beq %g", wvb);
} 


void wait_for_vblank(void)
{
	while (vblank_occured == 0)
	{
	};

	vblank_occured = 0;
}

unsigned char xpos;

void erase_sprite(void)
{
	POKE(SCREEN_MEM + xpos,0);

	// put data in player 0
    memset((void*)(PMG_MEM + 0x400 + player_ypos),0,sizeof(player0_data));

    // put data in player 1
    memset((void*)(PMG_MEM + 0x500 + player_ypos),0,sizeof(player1_data));

    // erase missiles
    memset((void*)(PMG_MEM + 0x300 + player_ypos),0,sizeof(missiles_data));
}

void update_sprite(void)
{
	if (moving_type == MOVING_UP)
	{
		player_ypos--;
		if (player_ypos < 30)
			player_ypos = 30;
		return;
	}
	
	if (moving_type == MOVING_DOWN)
	{
		player_ypos++;
		if (player_ypos > 225)
		{
			player_ypos = 225;
		}
		return;
	}
	
	if (moving_type == MOVING_RIGHT)
	{
		player_xpos++;
		if (player_xpos > 200)
		{
			player_xpos = 200;
		}
		return;
	}
	if (moving_type == MOVING_LEFT)
	{
		player_xpos--;
		if (player_xpos < 48)
		{
			player_xpos = 48;
		}
		return;
	}
}

void draw_sprite(void)
{
	POKE(SCREEN_MEM + xpos,33);

    // put data in player 0
    memcpy((void*)(PMG_MEM + 0x400 + player_ypos),&player0_data,sizeof(player0_data));

    // put data in player 1
    memcpy((void*)(PMG_MEM + 0x500 + player_ypos),&player1_data,sizeof(player1_data));

    memcpy((void*)(PMG_MEM + 0x300 + player_ypos + 20),&missiles_data,sizeof(missiles_data));

	GTIA_WRITE.hposp0 = player_xpos;	
	GTIA_WRITE.hposp1 = player_xpos;

	GTIA_WRITE.hposm0 = player_xpos - 4;
	GTIA_WRITE.hposm1 = player_xpos - 6;
	GTIA_WRITE.hposm2 = player_xpos - 8;
	GTIA_WRITE.hposm3 = player_xpos - 10;

}


unsigned char collision;
void check_collision(void)
{
	collision = PEEK(P0PF);
	if (collision != 0)
	{
		// game over!!
		asm("brk");
	}
}

void handle_input(void)
{
	st = (PIA.porta & 0x0f);
    st = st ^ 0x0f;

	if (st == JOY_BIT_RIGHT)
	{
		moving_type = MOVING_RIGHT;
		return;
	}
	
	if (st == JOY_NO_MOVE)
	{
		moving_type = MOVING_NONE;
		return;
	}

	if (st == JOY_BIT_LEFT)
	{
		moving_type = MOVING_LEFT;
		return;
	}

	if (st == JOY_BIT_UP)
	{
		moving_type = MOVING_UP;
		return;
	}
	
	if (st == JOY_BIT_DOWN)
	{
		moving_type = MOVING_DOWN;
	}

}
void dli_routine(void);

unsigned char ind = 0;
void dli_routine2(void)
{
	asm("pha");
    asm("txa");
    asm("pha");
    asm("tya");
    asm("pha");

	GTIA_WRITE.hposp0 = 0x95;
	ANTIC.wsync = 1;
	GTIA_WRITE.colpm0 = ind + 16;
	ind++;
	GTIA_WRITE.colbk = 0x00;


	GTIA_WRITE.colpm0 = ind + 16;
	ind++;
	ANTIC.wsync = 1;
	GTIA_WRITE.colpm0 = ind + 16;
	ind++;	
	ANTIC.wsync = 1;
	GTIA_WRITE.colpm0 = ind + 16;
	ind++;
	ANTIC.wsync = 1;
	GTIA_WRITE.colpm0 = ind + 16;
	ind++;
	ANTIC.wsync = 1;
	GTIA_WRITE.colpm0 = ind + 16;
	ind++;
	ANTIC.wsync = 1;
	GTIA_WRITE.colpm0 = ind + 16;
	ind++;
	
	

//	GTIA_WRITE.hposp1 = 0x95;


	OS.vdslst = &dli_routine;

    asm("pla");
    asm("tay");
    asm("pla");
    asm("tax");
    asm("pla");

    asm("rti");
}

void dli_routine1(void)
{
	asm("pha");
    asm("txa");
    asm("pha");
    asm("tya");
    asm("pha");

	ANTIC.wsync = 1;

	GTIA_WRITE.colbk = 0x00;

	OS.vdslst = &dli_routine2;

    asm("pla");
    asm("tay");
    asm("pla");
    asm("tax");
    asm("pla");

    asm("rti");
}

void dli_routine(void)
{
	asm("pha");
    asm("txa");
    asm("pha");
    asm("tya");
    asm("pha");

    ANTIC.wsync = 1;
	GTIA_WRITE.colpm0 = 0xB6;

	GTIA_WRITE.colbk = 0x00;
    ANTIC.chbase = 0xE0; //MSB of 0xE000
    GTIA_WRITE.colpf0 = 0x08;

    ANTIC.wsync = 1;
    ANTIC.wsync = 1;
    ANTIC.wsync = 1;
    ANTIC.wsync = 1;
    ANTIC.wsync = 1;
    ANTIC.wsync = 1;
    ANTIC.wsync = 1;
    ANTIC.wsync = 1;
    
    ANTIC.chbase = CHARSET_MEM >> 8; // MSB / 256
    
    GTIA_WRITE.colpf0 = 0x55;

    OS.vdslst = &dli_routine1;

	asm("pla");
    asm("tay");
    asm("pla");
    asm("tax");
    asm("pla");

    asm("rti");
}

void vbi_copy_shadow_to_hw_registers(void)
{
    ANTIC.dmactl = OS.sdmctl;

    ANTIC.dlistl = OS.sdlstl;
    ANTIC.dlisth = OS.sdlsth;

    ANTIC.chbase = OS.chbas;

    GTIA_WRITE.prior = OS.gprior;

    GTIA_WRITE.colpf0 = OS.color0;
    GTIA_WRITE.colpf1 = OS.color1;
    GTIA_WRITE.colpf2 = OS.color2;
    GTIA_WRITE.colpf3 = OS.color3;
    GTIA_WRITE.colbk  = OS.color4;
    
    GTIA_WRITE.colpm0 = OS.pcolr0;
    GTIA_WRITE.colpm1 = OS.pcolr1;
    GTIA_WRITE.colpm2 = OS.pcolr2;
    GTIA_WRITE.colpm3 = OS.pcolr3;  
}
void vbi_routine(void)
{
	vbi_copy_shadow_to_hw_registers();

	OS.vdslst = &dli_routine;

	vblank_occured = 1;

	asm("pla");
    asm("tay");
    asm("pla");
    asm("tax");
    asm("pla");

    asm("rti");
}

void init_vbi2(void)
{
	asm("lda #7");
	asm("ldx #>%v", vbi_routine);
	asm("ldy #<%v", vbi_routine);
	asm("jsr $E45C");
}


void init_vbi_dli(void)
{
	wait_for_vblank_clock();
	OS.vvblki = &vbi_routine;
}

void setup_pmg(void)
{	
	// tell ANTIC what is the memory address of the PMG memory
    ANTIC.pmbase = PMG_MEM >> 8;

    // clear PMG memory
	bzero((void*)(PMG_MEM+0x300),0x500);

    // tell ANTIC do we work with single line or double line resolution 
    OS.sdmctl = 0x3E; // single line

	// tell GTIA what type of PMG do we want?
	GTIA_WRITE.gractl = GRACTL_MISSLES + GRACTL_PLAYERS;

    // set the priority register (order of objects appear on screen)
	// PRIOR_P03_PF03 | PRIOR_5TH_PLAYER | PRIOR_OVERLAP_3RD_COLOR
    OS.gprior = PRIOR_P03_PF03 | PRIOR_OVERLAP_3RD_COLOR | PRIOR_5TH_PLAYER;

    // player colors , set shadows
    OS.pcolr0 = 0xB6;
    OS.pcolr1 = 0x18;
    OS.pcolr2 = 0x36;
    OS.pcolr3 = 0xCC;

    // missiles combining the 5th player get their color from location 711
    // which is playfield 3
	OS.color3 = 0x26; 

    // player size (normal, double, quad)
    GTIA_WRITE.sizem  = PMG_SIZE_NORMAL;	
    GTIA_WRITE.sizep0 = PMG_SIZE_NORMAL;
    GTIA_WRITE.sizep1 = PMG_SIZE_NORMAL;
    GTIA_WRITE.sizep2 = PMG_SIZE_NORMAL;
    GTIA_WRITE.sizep3 = PMG_SIZE_NORMAL;

    // player horizontal position
    GTIA_WRITE.hposm0 = 0x00;
    GTIA_WRITE.hposm1 = 0x00;
    GTIA_WRITE.hposm2 = 0x00;
    GTIA_WRITE.hposm3 = 0x00;

    GTIA_WRITE.hposp0 = 0x00;
    GTIA_WRITE.hposp1 = 0x00;
    GTIA_WRITE.hposp2 = 0x00;
    GTIA_WRITE.hposp3 = 0x00;

    player_ypos = 0x50;
    player_xpos = 0x50;


    memset((void*)(PMG_MEM + 0x400 + 0x90),255,8);
}

void main(void)
{
	moving_type = MOVING_NONE;
	vblank_occured = 0;
	xpos = 0;
	memcpy((void*)DLIST_MEM,antic4_display_list, sizeof(antic4_display_list));
	
	OS.sdlst = (void*)DLIST_MEM;

	OS.color0 = 0x55;		// playerfield 0
	OS.color1 = 0xA3;       // playerfield 1
	OS.color2 = 0X26;		// playerfield 2
	OS.color3 = 0xE1;		// playerfield 3
	OS.color4 = 0x00;		// background

	memcpy((void*)CHARSET_MEM, (void*)0xE000, 0x400);

	// [CHARSETADDRESS] + 8 * charcode
	memcpy((void*) (CHARSET_MEM + 8 * 33) , sprite_dat, 8);

	OS.chbas = CHARSET_MEM >> 8; // MSB / 256

	POKE(SCREEN_MEM + xpos,33);
	
	setup_pmg();

	init_vbi_dli();

	// start the DLI by adding it to the VBI with the NMIEN register
	ANTIC.nmien = NMIEN_DLI | NMIEN_VBI;


	while(1)
	{
		// wait for 1 frame to pass
		wait_for_vblank();

		handle_input();

		erase_sprite();
		update_sprite();
		draw_sprite();

		check_collision();
	};


	// stop DLI by settign NMIEN to point only on VBI
	ANTIC.nmien = NMIEN_VBI; 
}