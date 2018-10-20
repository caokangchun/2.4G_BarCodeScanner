#ifndef __BSPPLAY_H__
#define __BSPPLAY_H__

#include "gd32f1x0.h"

#include "debug.h"

#include "BSPScan.h"








#define BEEP_ENABLE		0
#define BEEP_DISABLE	1


#define HIGH	230
#define MID		300
#define LOW		370

void BSPPlaySuccess(void);
void BSPPlayFail(void);
void BSPPlayPowerOn(void);
void BSPPlayPowerOff(void) __attribute__((section ("BSPPlayPowerOff")));
void BSPPlay(void);



static void beep_init(void) __attribute__((section ("beep_init")));
void time1_init_pwm(void) __attribute__((section ("time1_init_pwm")));
void time0_init(void) __attribute__((section ("time0_init")));
void read_play_list(uint32_t *time, uint16_t *pitch);

void play_list_write(uint16_t pitch, uint32_t time);
void PlayListInit(void);









typedef struct
{
	uint32_t time;	//播放时间
	uint16_t pitch;	//音色
}Sound;


typedef struct
{
	uint8_t write;
	uint8_t read;
	Sound sound[4];
}tPlayList;

#endif
