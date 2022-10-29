#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "entity.h"
#include "gba/gba_input.h"
#include "utils/animation.h"

typedef struct PlayerEntityData
{
	EntityData Entity;
	AnimationUniqueData EntityAnim;

}PlayerEntityData;

extern void InitPlayer(PlayerEntityData* self, u16 tileIDX, u16 palIDX);
extern void HandlePlayerMove(AXIS axis, s16 value, void* data);
extern void UpdatePlayer(PlayerEntityData* self);

#endif