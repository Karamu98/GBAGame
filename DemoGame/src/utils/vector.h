#ifndef __VECTOR_H__
#define __VECTOR_H__
#include "../gba/gba_types.h"

typedef struct Vec2
{
	s32 X;
	s32 Y;
}Vec2;

typedef struct UVec2
{
	u32 X;
	u32 Y;
}UVec2;

extern Vec2 AddVec2(const Vec2* first, const Vec2* second);
extern Vec2 SubVec2(const Vec2* first, const Vec2* second);

extern UVec2 AddUVec2(const UVec2* first, const UVec2* second);

#endif