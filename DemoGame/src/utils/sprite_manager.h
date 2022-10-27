#ifndef __SPRITE_MANAGER_H__
#define __SPRITE_MANAGER_H__

#include "../gba/gba_math.h"
#include "../gba/gba_gfx.h"

#define INVALID_SPRITE_ID 0xFF

typedef struct Sprite
{
	u8 ID;
	u8* XPos;
	u8* YPos;
	fv2 Size;

	SpriteObject* Attributes;
}Sprite;

extern void InitSprite(Sprite* self, u8 id, fv2 pos, fv2 size);

extern void SetSpritePosition(u8 spriteID, fv2 newPos);
extern fv2 GetSpritePosition(u8 spriteID);
extern void SetSpriteTileIDX(u8 spriteID, u16 tileIDX);
extern u16 GetSpriteTileIDX(u8 spriteID);

extern u8 GetFreeSprite();
extern u8 SpriteIDToIndex(u8 spriteID);
extern void TagSprite(u8 spriteID);
extern void DropSprite(u8 spriteID);


#endif