#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "utils/sprite_manager.h"
#include "utils/animation.h"

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


#endif