#ifndef __COLLISION_H__
#define __COLLISION_H__

#include "../gba/gba_types.h"

typedef struct Rect
{
	u16 X;
	u16 Y;
	u16 W;
	u16 H;
}Rect;

extern bool IsRectOverlapped(const Rect* self, const Rect* other);

#endif