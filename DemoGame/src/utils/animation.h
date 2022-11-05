#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "../gba/gba_types.h"
#include "texture.h"
#include "sprite_manager.h"


typedef struct AnimationUniqueData
{
	u8 TicksPerFrame; // TODO: Improve
	Sequence* Sequence;
	u8 FrameOffset;

	Texture* TargetTexture;
	u16* MemTarget;

	u8 CurFrame; // Mod with len for sequence
	u8 _speedTick; // Timer for incrementing _frameTick
	u32 _dataLen;
}AnimationUniqueData;

typedef struct AnimationSharedData
{
	u8 TicksPerFrame; // TODO: Improve
	Sequence* Sequence;
	u8 FrameOffset;

	u16 StartTileIDX;
	Sprite* sprite;

	u8 CurFrame; // Mod with len for sequence
	u8 _speedTick; // Timer for incrementing _frameTick
}AnimationSharedData;

extern void InitUniqueAnimation(AnimationUniqueData* self, u16* memTarget, Texture* target, Sequence* sequence, u8 ticksPerFrame);
extern void UpdateUniqueAnimation(AnimationUniqueData* self);
extern void DrawUniqueAnimation(AnimationUniqueData* self);

extern void InitSharedAnimation(AnimationSharedData* self, Sprite* sprite, u16 startTileIDX, Sequence* sequence, u8 ticksPerFrame);
extern void UpdateSharedAnimation(AnimationSharedData* self);
extern void DrawSharedAnimation(AnimationSharedData* self);



#endif