#include "animation.h"
#include <string.h>

void InitUniqueAnimation(AnimationUniqueData* self, u16* memTarget, Texture* target, Sequence* sequence, u8 ticksPerFrame)
{
	self->MemTarget = memTarget;
	self->TargetTexture = target;
	self->Sequence = sequence;
	self->TicksPerFrame = ticksPerFrame;

	self->_speedTick = 0;
	self->CurFrame = 0;
	if(target->Tiles->Is4Bpp)
	{
		self->_dataLen = (target->FrameWidth * target->FrameWidth) * 0.5f;
	}
	else
	{
		self->_dataLen = (target->FrameWidth * target->FrameWidth);
	}
}

void UpdateUniqueAnimation(AnimationUniqueData* self)
{
	if(self->_speedTick == 0)
	{
		self->_speedTick = self->TicksPerFrame;
		++self->CurFrame;
	}
	--self->_speedTick;
}

void DrawUniqueAnimation(AnimationUniqueData* self)
{
	u8 seqIDX = self->CurFrame % self->Sequence->Len;
	u8 curFrame = self->Sequence->Data[seqIDX];
	curFrame += self->FrameOffset;
	u16 offset = curFrame * self->_dataLen;
	memcpy(self->MemTarget, self->TargetTexture->Tiles->Data + (u16)(offset * 0.5f), self->_dataLen); // Copy character tiles into Tile memory block 4, start
}

void InitSharedAnimation(AnimationSharedData* self, Sprite* sprite, u16 startTileIDX, Sequence* sequence, u8 ticksPerFrame)
{
	self->sprite = sprite;
	self->StartTileIDX = startTileIDX;
	self->Sequence = sequence;
	self->TicksPerFrame = ticksPerFrame;

	self->_speedTick = 0;
	self->CurFrame = 0;
}

void UpdateSharedAnimation(AnimationSharedData* self)
{
	if(self->_speedTick == 0)
	{
		self->_speedTick = self->TicksPerFrame;
		++self->CurFrame;
	}
	--self->_speedTick;
}

void DrawSharedAnimation(AnimationSharedData* self)
{
	u8 seqIDX = self->CurFrame % self->Sequence->Len;
	u8 curFrame = self->Sequence->Data[seqIDX];
	curFrame += self->FrameOffset;
	SetSpriteTileIDX(self->sprite->ID, self->StartTileIDX + curFrame);
}