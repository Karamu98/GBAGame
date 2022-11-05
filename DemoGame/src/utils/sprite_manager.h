#ifndef __SPRITE_MANAGER_H__
#define __SPRITE_MANAGER_H__

#include "../gba/gba_gfx.h"
#include "texture.h"
#include "camera.h"

#define INVALID_SPRITE_ID 0xFF

typedef struct Sprite
{
	u8 ID;
	u8* ScreenXPos;
	u8* ScreenYPos;
	Transform Transform;

	SpriteObject* _attributes;
}Sprite;

extern void InitSprite(Sprite* self, Vec2 worldPosition, u8 id, Texture* texRef, u16 tileIDX, u16 palIDX, bool loadTiles, bool loadPal);

extern void SetSpriteScreenPosition(u8 spriteID, u16 screenX, u8 screenY);
extern UVec2 GetSpriteScreenPosition(u8 spriteID);
extern void SetSpriteTileIDX(u8 spriteID, u16 tileIDX);
extern u16 GetSpriteTileIDX(u8 spriteID);
extern void SetSpritePalIDX(u8 spriteID, u8 palIDX);
extern u8 GetSpritePalIDX(u8 spriteID);

extern u8 GetFreeSprite();
extern u8 SpriteIDToIndex(u8 spriteID);
extern void TagSprite(u8 spriteID);
extern void DropSprite(u8 spriteID);

extern void SetSpriteHidden(u8 spriteID, bool isHidden);

extern void DrawSprite(Sprite* self, Camera* activeCamera);


#endif