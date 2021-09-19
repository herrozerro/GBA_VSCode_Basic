#ifndef __GBA_INPUT_H__
#define __GBA_INPUT_H__

#include "gba_types.h"
#include "gba_constants.h"



extern u16 __currKeys = 0, __prevKeys = 0;

inline void PollKeys()
{
	__prevKeys = __currKeys;
	__currKeys = ~REG_KEYINPUT_STATUS;
}

inline u16		currentKeyState()           { return __currKeys; }
inline u16		prevKeyState()              { return __prevKeys; }

inline u16		keyDown(u16 a_key)       { return __currKeys & a_key; }
inline u16		keyUp(u16 a_key)         { return ~__currKeys & a_key; }

inline u16		keyHeld(u16 a_key)       { return (__currKeys & __prevKeys) & a_key; }
inline u16		keyReleased(u16 a_key)   { return (~__currKeys & __prevKeys) & a_key; }
inline u16		keyHit(u16 a_key)        { return (__currKeys & ~__prevKeys) & a_key; }

inline u16		keyStateChange(u16 a_key){ return (__currKeys ^ __prevKeys) & a_key; }


#endif