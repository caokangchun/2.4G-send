#ifndef	__BEEP_H__
#define	__BEEP_H__


#include "Common.h"
#include "BSP.h"


#define		MUSIC_TRACK_1				0x00000000


extern void BSPSoundInit(uint8_t Volume);
extern void BSPSoundPlay(uint32_t Track);
extern void BSPSoundStop(void);


#endif
