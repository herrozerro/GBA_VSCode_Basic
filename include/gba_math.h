#pragma once

#ifndef __GBA_MATH_H__
#define __GBA_MATH_H__

#include "gba_types.h"


s16 abs_s16(s16 a_val)
{
	s16 mask = a_val >> 31;
	return (a_val ^ mask) - mask;
}
s32 abs_s32(s32 a_val)
{
	s32 mask = a_val >> 31;
	return (a_val ^ mask) - mask;
}

inline u16 clamp_u16(u16 value, u16 min, u16 max)
{
    if (value < min)
        value = min;

    if (value > max)
        value = max;

     return value;
}
inline u32 clamp_u32(u32 value, u32 min, u32 max)
{
    if (value < min)
        value = min;

    if (value > max)
        value = max;

     return value;
}
inline s16 clamp_s16(s16 value, s16 min, s16 max)
{
    if (value < min)
        value = min;

    if (value > max)
        value = max;

     return value;
}
inline s32 clamp_s32(s32 value, s32 min, s32 max)
{
    if (value < min)
        value = min;

    if (value > max)
        value = max;

     return value;
}


#endif
