#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "../gba/gba_types.h"

typedef struct AnimationData
{
	u8 TicksPerFrame; // TODO: Improve
	u8* Sequence;
	u8 Length;

	u8 FrameTileCount;
	u8 FrameOffset;

	u8 _frameTick; // Mod with len for sequence
	u8 _speedTick; // Timer for incrementing _frameTick
}AnimationData;

extern void InitAnimation(AnimationData* self, u8 tpf, u8* seq, u8 len, u8 tileCount, u8 offset);
extern void UpdateAnimation(AnimationData* animData);
extern u16 EvaluateAnimation(AnimationData* animData);



#endif