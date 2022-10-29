#include "player.h"
#include "utils/texture.h"


const u8 S_TEMP_ANIM_MAP[] = {2, 1, 3, 0};

void InitPlayer(PlayerEntityData* self, u16 tileIDX, u16 palIDX)
{
	InitEntity(&self->Entity, 10, 60, &S_Textures[CHARACTER_SHEET_ASSET], tileIDX, palIDX);
	//BindActionToAxis(HORIZONTAL, &HandlePlayerMove, self->Entity.Sprite.XPos);
	//BindActionToAxis(VERTICAL, &HandlePlayerMove, self->Entity.Sprite.YPos);
}

void HandlePlayerMove(AXIS axis, s16 value, void* data)
{
	u8* target = (u8*)data;
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