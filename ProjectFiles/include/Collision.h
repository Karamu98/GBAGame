#ifndef _COLLISION_H_
#define _COLLISION_H_

#include "fv2.h"
#include "Sprite.h"


// AABB
extern bool AABBDynCollision(DynSprite* a_sprite1, DynSprite* a_sprite2); // Tests two moving sprites to test for their collision
extern bool AABBStaticCollision(DynSprite* a_sprite, Sprite* a_staticsprite); // Tests a moving sprite against a static sprite testing for collision

// Circular
extern bool DynCircleCollision(DynSprite* a_circleSprite, DynSprite* a_sprite); // Tests a circular moving sprite against a moving sprite
extern bool StaticCircleCollision(DynSprite* a_circleSprite, Sprite* a_sprite); // Tests a circular moving sprite against a static sprite

// Other



#endif