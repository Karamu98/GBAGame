#ifndef __SPRITE_MANAGER_H__
#define __SPRITE_MANAGER_H__

#include "../gba/gba_gfx.h"
#include "texture.h"
#include "transform.h"

#define INVALID_SPRITE_ID 0xFF

typedef struct Sprite
{
	u8 ID;
	u8* ScreenXPos;
	u8* ScreenYPos;
	Transform Transform;

	SpriteObject* _attributes;
}Sprite;

extern void InitSprite(Sprite* self, u8 id, u8 x, u8 y, Texture* texRef, u16 tileIDX, u16 palIDX);

extern void SetSpritePosition(u8 spriteID, u8 x, u8 y);
extern fv2 GetSpritePosition(u8 spriteID);
extern void SetSpriteTileIDX(u8 spriteID, u16 tileIDX);
extern u16 GetSpriteTileIDX(u8 spriteID);
extern void SetSpritePalIDX(u8 spriteID, u8 palIDX);
extern u8 GetSpritePalIDX(u8 spriteID);

extern u8 GetFreeSprite();
extern u8 SpriteIDToIndex(u8 spriteID);
extern void TagSprite(u8 spriteID);
extern void DropSprite(u8 spriteID);


#endif