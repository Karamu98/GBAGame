#ifndef _SPRITE_H_
#define _SPRITE_H_

#include "fv2.h"
#include "gba_gfx.h"

typedef struct Sprite
{
	u8 SpriteID; // Stores ID
	u16 StartTileID; // Stores start tile

	fv2 vWorldPos;
	fv2 vPosition; // Stores Position
	fv2 vSize; // Width, Height

	ObjectAttribute Attributes; // Stores attribs to be applied to OAM
}Sprite;

typedef struct DynSprite
{
	u8 SpriteID; // Stores ID
	u16 StartTileID; // Stores start tile

	//fv2 vWorldPos;
	fv2 vPosition; // Stores Position
	fv2 vSize; // Width, Height
	fv2 vVelocity; // Stores Velocity

	ObjectAttribute Attributes; // Stores attribs to be applied to OAM
}DynSprite;

#endif