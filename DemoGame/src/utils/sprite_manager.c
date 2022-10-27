#include "sprite_manager.h"

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

void InitSprite(Sprite* self, u8 id, fv2 pos, fv2 size)
{
	self->ID = id;
	self->Size = size;
	u8 objIDX = SpriteIDToIndex(id);
	self->Attributes = &obj_buffer[objIDX];

	SetSpritePosition(id, pos);

	self->YPos = (u8*)&self->Attributes->attr0;
	self->XPos = (u8*)&self->Attributes->attr1;

	TagSprite(id);
}

void SetSpritePosition(u8 spriteID, fv2 newPos)
{
	SpriteObject* curObj = &obj_buffer[SpriteIDToIndex(spriteID)];
	curObj->attr0 = BitSetU16ByMask(curObj->attr0, fix2uint(newPos.Y), A0_YPOS_MASK);
	curObj->attr1 = BitSetU16ByMask(curObj->attr1, fix2uint(newPos.X), A1_XPOS_MASK);
}

fv2 GetSpritePosition(u8 spriteID)
{
	u8 y = (u8)obj_buffer[SpriteIDToIndex(spriteID)].attr0;
	u8 x = (u8)obj_buffer[SpriteIDToIndex(spriteID)].attr1;
	return (fv2){x, y};
}

void SetSpriteTileIDX(u8 spriteID, u16 tileIDX)
{
	SpriteObject* curObj = &obj_buffer[SpriteIDToIndex(spriteID)];
	curObj->attr2 = BitSetU16ByMask(curObj->attr0, tileIDX, A2_TILE_MASK);
}

u16 GetSpriteTileIDX(u8 spriteID)
{
	SpriteObject* curObj = &obj_buffer[SpriteIDToIndex(spriteID)];
	return BitGetU16ByMask(curObj->attr2, A2_TILE_MASK);
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
