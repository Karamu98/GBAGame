#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "utils/sprite_manager.h"

typedef struct EntityData
{
	Sprite Sprite;
	u16 Colour;
}EntityData;

extern void InitEntity(EntityData* self, u16 x, u16 y, u16 w, u16 h, u16 colour);



#endif