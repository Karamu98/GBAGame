#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "entity.h"
#include "gba/gba_input.h"
#include "utils/animation.h"

extern void InitPlayer(EntityData* self);
extern void HandlePlayerMove(AXIS axis, s16 value, void* data);
extern void UpdatePlayer(EntityData* self, AnimationUniqueData* anim);

#endif