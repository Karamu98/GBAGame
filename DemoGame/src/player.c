#include "player.h"
#include "utils/texture.h"
#include "utils/input.h"


const u8 S_TEMP_ANIM_MAP[] = {2, 1, 3, 0};

void InitPlayer(EntityData* self)
{
	BindActionToAxis(HORIZONTAL, &HandlePlayerMove, &self->Sprite.Transform.Position.X);
	BindActionToAxis(VERTICAL, &HandlePlayerMove, &self->Sprite.Transform.Position.Y);
}

void HandlePlayerMove(AXIS axis, s16 value, void* data)
{
	s32* target = (s32*)data;
	*target += axis == HORIZONTAL ? value : -value;
}

void UpdatePlayer(EntityData* self, AnimationUniqueData* anim)
{
	Direction* newDir = &self->ForwardDirection;
	if(keyDown(LEFT)) {*newDir = DIR_Left;}
	if(keyDown(RIGHT)) {*newDir = DIR_Right;}
	if(keyDown(UP)) {*newDir = DIR_Up;}
	if(keyDown(DOWN)) {*newDir = DIR_Down;}

	if(keyDown(LEFT | RIGHT | UP | DOWN))
	{
		UpdateUniqueAnimation(anim);
	}
	else
	{
		anim->CurFrame = 0;
	}

	anim->FrameOffset = S_TEMP_ANIM_MAP[*newDir] * S_Textures[CHARACTER_SHEET_TEXTURE_ASSET].FramesPerRow;
}