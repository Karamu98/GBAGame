#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "Sprite.h"
#include "ParticleSystem.h"
#include "Projectile.h"
#include "Player.h"


typedef enum EnemyAnimState
{
	MOVELEFT,
	MOVEDOWN,
	MOVEUP,
	MOVERIGHT
}EnemyAnimState;


#define MAX_ENEMIES 4

#define KNIGHT_HEALTH 100
#define KNIGHT_DAMAGE 15
#define KNIGHT_SPEED 1

// ========================= Defaults =========================
typedef struct Enemy
{
	u8 iHealth;
	u8 iDamage;

	EnemyAnimState AnimState;
	u8 currentFrame;
	u8 animCounter;
	u8 animationSpeed;
	u8 enemyID; // Stores ID of enemy to stop over spawninng
	u8 enemyType; // 0 = Skeleton, 1 = Mage
	bool bActive; // If the enemy is active
	bool bAlive; // If the enemy is alive

	u8 iSpeed;
	u8 bXAxis;

	DynSprite Sprite;
}Enemy;

extern Enemy* g_enemyArray; // Stores all enemies


extern fv2 g_mapPos;

extern Player* g_PlayerObj;


// ========================= Functions =========================


// Spawning
extern void InitialiseEnemies();

extern void SpawnKnight(Enemy* a_enemy);

// Data Control
extern void DamageKnight(Enemy* a_enemy); // Damages passed enemy

// Updating
extern void UpdateKnight(Enemy* a_enemy); // Updates goblin type enemies
extern void KnightAnimator(Enemy *a_knight);






#endif

