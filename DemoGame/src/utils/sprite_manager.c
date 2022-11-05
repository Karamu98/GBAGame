#include "sprite_manager.h"
#include "../gba/gba_drawing.h"
#include "../gba/gba_mathUtil.h"

#include "bitwise.h"

u8 S_SpriteFreeFlags[16];

u8 GetFreeSprite()
{
	for (u8 i = 0; i < 16; i++)
	{
		for (u8 j = 0; j < 8; j++)
		{
			u8 mask = (S_SpriteFreeFlags[i] & (1 << j));
			if (!mask) 
			{
				return (i << 4) | j; 
			}
		}
	}

	return INVALID_SPRITE_ID;
}

u8 SpriteIDToIndex(u8 spriteID)
{
	u8 arrayPos = spriteID >> 4;
	u8 bitPos = spriteID & 0xF;

	u8 final = (8 * arrayPos) + bitPos;

	return final;
}

static u8 _attSizeLUT[][4][2]=
{
	{ // SQUARE
		{8, 8},
		{16, 16},
		{32, 32},
		{64, 64}
	},
	{ // HORIZONTAL
		{16, 8},
		{32, 8},
		{32, 16},
		{64, 32}
	},
	{ // Vertical
		{8, 16},
		{8, 32},
		{16, 32},
		{32, 64}
	}
};

void InitSprite(Sprite* self, Vec2 worldPosition, u8 id, Texture* texRef, u16 tileIDX, u16 palIDX, bool loadTiles, bool loadPal)
{
	TagSprite(id);

	if(loadTiles)
	{
		LoadSpriteTiles(texRef->Tiles, tileIDX, 0);
	}

	if(loadPal)
	{
		
	}

	self->ID = id;
	self->_attributes = &obj_buffer[SpriteIDToIndex(id)];
	self->ScreenYPos = (u8*)&self->_attributes->attr0;
	self->ScreenXPos = (u8*)&self->_attributes->attr1;
	self->Transform.Position = worldPosition;

	u8 _ObjectMode = A0_MODE_REG;
	u8 _GraphicsMode = A0_GFX_MODE_REG;
	u8 _ColourMode = texRef->Tiles->Is4Bpp ? A0_COLOUR_MODE_4BPP : A0_COLOUR_MODE_8BPP;
	u8 _Shape = texRef->FrameWidth == texRef->FrameHeight ? A0_SHAPE_SQUARE : texRef->FrameWidth < texRef->FrameHeight ? A0_SHAPE_TALL : A0_SHAPE_WIDE;
	u8 _Size = 0;
	for(int i = 0; i < 4; ++i)
	{
		u8* cur = _attSizeLUT[_Shape][i];
		if(cur[0] == texRef->FrameWidth && cur[1] == texRef->FrameHeight)
		{
			_Size = i;
			break;
		}
	}

	self->_attributes->attr0 = SetSpriteObjectAttrib0(0, _ObjectMode, _GraphicsMode, 0, _ColourMode, _Shape);
	self->_attributes->attr1 = SetSpriteObjectAttrib1(0, 0, _Size);
	self->_attributes->attr2 = SetSpriteObjectAttrib2(tileIDX, A2_PRIORITY_0, palIDX);
}

void SetSpriteScreenPosition(u8 spriteID, u16 screenX, u8 screenY)
{
	SpriteObject* curObj = &obj_buffer[SpriteIDToIndex(spriteID)];
	curObj->attr0 = BitSetU16ByMask(curObj->attr0, screenY, A0_YPOS_MASK);
	curObj->attr1 = BitSetU16ByMask(curObj->attr1, screenX, A1_XPOS_MASK);
}

UVec2 GetSpriteScreenPosition(u8 spriteID)
{
	u8 screenY = (u8)obj_buffer[SpriteIDToIndex(spriteID)].attr0;
	u16 screenX = (u16)obj_buffer[SpriteIDToIndex(spriteID)].attr1;
	return (UVec2){screenX, screenY};
}

void SetSpriteTileIDX(u8 spriteID, u16 tileIDX)
{
	SpriteObject* curObj = &obj_buffer[SpriteIDToIndex(spriteID)];
	curObj->attr2 = BitSetU16ByMask(curObj->attr2, tileIDX, A2_TILE_MASK);
}

u16 GetSpriteTileIDX(u8 spriteID)
{
	SpriteObject* curObj = &obj_buffer[SpriteIDToIndex(spriteID)];
	return BitGetU16ByMask(curObj->attr2, A2_TILE_MASK);
}

void SetSpritePalIDX(u8 spriteID, u8 palIDX)
{
	SpriteObject* curObj = &obj_buffer[SpriteIDToIndex(spriteID)];
	curObj->attr2 = BitSetU16ByMask(curObj->attr2, palIDX, A2_PALETTE_MASK);
}

u8 GetSpritePalIDX(u8 spriteID)
{
	SpriteObject* curObj = &obj_buffer[SpriteIDToIndex(spriteID)];
	return BitGetU16ByMask(curObj->attr2, A2_PALETTE_MASK);
}

void TagSprite(u8 spriteID)
{
	u8 arrayPos = spriteID >> 4;
	u8 bitPos = spriteID & 0xF;

	u8 mask = (1 << bitPos);

	S_SpriteFreeFlags[arrayPos] = (S_SpriteFreeFlags[arrayPos] | mask); // Apply to u8 value
}

void DropSprite(u8 spriteID)
{
	u8 arrayPos = spriteID >> 4;
	u8 bitPos = spriteID & 0xF0;

	u8 mask = 1 << bitPos; 

	S_SpriteFreeFlags[arrayPos] = S_SpriteFreeFlags[arrayPos] & (~mask); // Apply to u8 value
}

void SetSpriteHidden(u8 spriteID, bool isHidden)
{
	SpriteObject* curObj = &obj_buffer[SpriteIDToIndex(spriteID)];
	if(!isHidden) {UnhideSpriteObject(curObj, 0);} else {HideSpriteObject(curObj);}
}

void DrawSprite(Sprite* self, Camera* activeCamera)
{
	Vec2 screenPos = SubVec2(&self->Transform.Position, &activeCamera->Transform.Position);

	// Are we close enough to be on screen
	if(screenPos.X >= 0 && screenPos.Y >= 0 && screenPos.X < (SCREEN_W + SCREEN_BLANK_HALF) && screenPos.Y < (SCREEN_H + SCREEN_BLANK_HALF))
	{
		u16 newX = ((u16)screenPos.X) % (SCREEN_W + SCREEN_BLANK_HALF);
		u8 newY = ((u16)screenPos.Y) % (SCREEN_H + SCREEN_BLANK_HALF);

		SetSpriteScreenPosition(self->ID, newX, newY);
		SetSpriteHidden(self->ID, false);
	}
	else
	{
		SetSpriteHidden(self->ID, true);
	}
}