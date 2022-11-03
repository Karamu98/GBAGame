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

const u8 _attSizeLUT[][4][2]=
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

void InitSprite(Sprite* self, u8 id, u8 x, u8 y, Texture* texRef, u16 tileIDX, u16 palIDX)
{
	TagSprite(id);

	self->ID = id;
	self->_attributes = &obj_buffer[SpriteIDToIndex(id)];
	self->YPos = (u8*)&self->_attributes->attr0;
	self->XPos = (u8*)&self->_attributes->attr1;

	u8 _ObjectMode = A0_MODE_REG;
	u8 _GraphicsMode = A0_GFX_MODE_REG;
	u8 _ColourMode = texRef->Is4Bpp ? A0_COLOUR_MODE_4BPP : A0_COLOUR_MODE_8BPP;
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

	self->_attributes->attr0 = SetSpriteObjectAttrib0(y, _ObjectMode, _GraphicsMode, 0, _ColourMode, _Shape);
	self->_attributes->attr1 = SetSpriteObjectAttrib1(x, 0, _Size);
	self->_attributes->attr2 = SetSpriteObjectAttrib2(tileIDX, A2_PRIORITY_0, palIDX); // TODO: Manage Palette and tile alloc
}

void SetSpritePosition(u8 spriteID, u8 x, u8 y)
{
	SpriteObject* curObj = &obj_buffer[SpriteIDToIndex(spriteID)];
	curObj->attr0 = BitSetU16ByMask(curObj->attr0, y, A0_YPOS_MASK);
	curObj->attr1 = BitSetU16ByMask(curObj->attr1, x, A1_XPOS_MASK);
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
