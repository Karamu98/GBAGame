#include "player.h"
#include "utils/texture.h"
#include "utils/input.h"


const u8 S_TEMP_ANIM_MAP[] = {2, 1, 3, 0};

void InitPlayer(PlayerEntityData* self, u16 tileIDX, u16 palIDX)
{
	InitEntity(&self->Entity, 0, 0, &S_Textures[CHARACTER_SHEET_ASSET], tileIDX, palIDX);
	InitUniqueAnimation(&self->EntityAnim, sprite_tile_address(0), &S_Textures[CHARACTER_SHEET_ASSET], &S_Sequences[CHARACTER_SHEET_ASSET], 7);

	BindActionToAxis(HORIZONTAL, &HandlePlayerMove, &self->Entity.Sprite.Transform.Position.X);
	BindActionToAxis(VERTICAL, &HandlePlayerMove, &self->Entity.Sprite.Transform.Position.Y);
}

void HandlePlayerMove(AXIS axis, s16 value, void* data)
{
	s32* target = (s32*)data;
	*target += axis == HORIZONTAL ? value : -value;
}

void UpdatePlayer(PlayerEntityData* self)
{
	Direction* newDir = &self->Entity.ForwardDirection;
	if(keyDown(LEFT)) {*newDir = DIR_Left;}
	if(keyDown(RIGHT)) {*newDir = DIR_Right;}
	if(keyDown(UP)) {*newDir = DIR_Up;}
	if(keyDown(DOWN)) {*newDir = DIR_Down;}

	if(keyDown(LEFT | RIGHT | UP | DOWN))
	{
		UpdateUniqueAnimation(&self->EntityAnim);
	}
	else
	{
		self->EntityAnim.CurFrame = 0;
	}

	self->EntityAnim.FrameOffset = S_TEMP_ANIM_MAP[*newDir] * S_Textures[CHARACTER_SHEET_ASSET].FramesPerRow;
	DrawUniqueAnimation(&self->EntityAnim);
}