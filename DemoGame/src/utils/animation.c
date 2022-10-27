#include "animation.h"

void InitAnimation(AnimationData* self, u8 tpf, u8* seq, u8 len, u8 tileCount, u8 offset)
{
	self->TicksPerFrame = tpf;
	self->Sequence = seq;
	self->Length = len;
	self->FrameTileCount = tileCount;
	self->FrameOffset = offset;

	self->_speedTick = 0;
	self->_frameTick = 0;
}

void UpdateAnimation(AnimationData* animData)
{
	if(animData->_speedTick == 0)
	{
		animData->_speedTick = animData->TicksPerFrame;
		++animData->_frameTick;
	}
	--animData->_speedTick;
}
u16 EvaluateAnimation(AnimationData* animData)
{
	return animData->FrameOffset + (animData->Sequence[animData->_frameTick % animData->Length] * animData->FrameTileCount);
}