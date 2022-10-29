#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "utils/sprite_manager.h"

typedef enum Direction
{
	DIR_Right, 
	DIR_Left,
	DIR_Up,
	DIR_Down,
}Direction;

typedef struct EntityData
{
	Sprite Sprite;
	Direction ForwardDirection;
}EntityData;

extern void InitEntity(EntityData* self, u8 x, u8 y, Texture* texRef, u16 tileIDX, u16 palIDX);



#endif