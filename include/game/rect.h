#pragma once

#ifndef __RECT_H
#define __RECT_H

#include "gba_types.h"

typedef struct RectInfo {

    s16 X;
    s16 Y;
    u16 Width;
    u16 Height;


} RectInfo;


bool rectOverlaps(const RectInfo* left, const RectInfo* right)
{
    return 
        left->X < right->X + right->Width && 
        left->X + left->Width > right->X && 
        left->Y < right->Y + right->Height && 
        left->Y + left->Height > right->Y;
}

#endif