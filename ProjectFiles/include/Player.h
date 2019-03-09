#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "fv2.h"
#include "Sprite.h"
#include "gba_input.h"
#include "Projectile.h"

typedef enum AnimationState 
{
	IDLEDOWN,
	IDLEUP,
	IDLELEFT,
	IDLERIGHT,
	WALKLEFT,
	WALKDOWN,
	WALKUP,
	WALKRIGHT,
	RUNDOWN,
	RUNUP,
	RUNLEFT,
	RUNRIGHT
}AnimationState;

typedef struct Player
{
	// Player Data
	DynSprite PlayerSprite; // Dynamic Sprite, holds position, velocity, size and OAMAttributes
	s8 iHealth; // Value that will determine the players bAlive state
	bool bAlive; // To test if the player is alive
	u8 iKills; // Keeps track of the players kills
	s32 iSpeed; // How fast the player moves (Speed * Velocity)
	fv2 vLastVel; // Stores the players last velocity

	// Animation Data
	AnimationState AnimState; // Stores the animation the player should be performing
	s16 animCounter; // Counts up to animationSpeed to animate for walking
	u8 currentFrame; // Which frame to load into tile memory
	s16 animationSpeed; // Max value that animCounter counts up to

	// Bullet Data
	u8 iProjectileCooldown; // Time between shots
	u8 iProjectileCounter; // To time between player bullets
	Projectile bullets[4]; // Player has "5" bullets to use
	u8 bulletCount; // Records how many bullets the player has, used for iterations

}Player;


extern fv2 g_mapPos;

extern void InitialisePlayer(Player *a_player); // Initialises the player, their paritcles, their bullets and the bullets particles
extern void HandleGameInput(Player *a_player); // Will handle the players input and processing of player data
extern void SetPlayerSprite(Player *a_player, u16 attr0, u16 attr1, u16 attr2); // Ease of use to set the players OBJAttribs
extern void DamagePlayer(Player* a_player);


#endif