#include "player.h"

#include "res/CharacterSheet.h"

static u8 S_DIRECTION;

#define CHAR_DOWN	0
#define CHAR_LEFT	1
#define CHAR_RIGHT	2
#define CHAR_UP 	3

void InitPlayer(PlayerEntityData* self)
{
	memcpy(PAL_SP_MEM, CharacterSheetPal, CharacterSheetPalLen); // Copy character pallet into Pallete sprite mem
	memcpy(&TILE_MEM[4][0], CharacterSheetTiles, CharacterSheetTilesLen); // Copy character tiles into Tile memory block 4, start

	InitAnimation(&self->EntityAnim, 15, CharacterSheetSequence, CharacterSheetSequenceLen, 16, 0);
	InitEntity(&self->Entity, 10, 60, 8, 40, setColor(0, 0, 31));

	self->Entity.Sprite.Attributes->attr0 = SetSpriteObjectAttrib0(*self->Entity.Sprite.YPos, A0_MODE_REG, A0_GFX_MODE_REG, 0, A0_COLOUR_MODE_4BPP, A0_SHAPE_SQUARE);
	self->Entity.Sprite.Attributes->attr1 = SetSpriteObjectAttrib1(*self->Entity.Sprite.XPos, 0, A1_SIZE_2);
	self->Entity.Sprite.Attributes->attr2 = SetSpriteObjectAttrib2(0, A2_PRIORITY_0, 0);

	BindActionToAxis(HORIZONTAL, &HandlePlayerMove, self->Entity.Sprite.XPos);
	BindActionToAxis(VERTICAL, &HandlePlayerMove, self->Entity.Sprite.YPos);
}

void HandlePlayerMove(AXIS axis, s16 value, void* data)
{
	u8* target = (u8*)data;
	if(axis == HORIZONTAL)
	{
		if(value < 0)
		{
			S_DIRECTION = CHAR_LEFT;
		} 
		else if(value > 0)
		{
			S_DIRECTION = CHAR_RIGHT;
		}
		*target += value;
	}
	else
	{
		if(value < 0)
		{
			S_DIRECTION = CHAR_DOWN;
		} 
		else if(value > 0)
		{
			S_DIRECTION = CHAR_UP;
		}
		*target -= value;
	}
}

void UpdatePlayer(PlayerEntityData* self)
{
	if(keyDown(LEFT | RIGHT | UP | DOWN))
	{
		UpdateAnimation(&self->EntityAnim);
	}
	else
	{
		self->EntityAnim._frameTick = 0;
	}

	self->EntityAnim.FrameOffset = (16 * S_DIRECTION) * CharacterFramesPerRow;
	SetSpriteTileIDX(self->Entity.Sprite.ID, EvaluateAnimation(&self->EntityAnim));
}