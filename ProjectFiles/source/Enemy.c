#include "Enemy.h"
#include "MemoryManager.h"
#include <string.h>
#include "Collision.h"

#include "../images/Knight.h"
#include "../images/Mage.h"


void InitialiseEnemies()
{
	for (int i = 0; i < MAX_ENEMIES; i++)
	{
		g_enemyArray[i].Sprite.SpriteID = GetFreeSprite();
		TagSprite(g_enemyArray[i].Sprite.SpriteID);

		g_enemyArray[i].Sprite.StartTileID = GetFreeTileWSpace(16);
		TagTile(g_enemyArray[i].Sprite.StartTileID, g_enemyArray[i].Sprite.StartTileID + 16);
	}

}

void SpawnKnight(Enemy* a_enemy)
{

	for (int i = 0; i < MAX_ENEMIES; i++)
	{
		if (!g_enemyArray[i].bActive)
		{
			// Set position
			g_enemyArray[i].Sprite.vPosition.fX = int2fix(qran_range(240, 276)); // Outside screen space
			g_enemyArray[i].Sprite.vPosition.fY = int2fix(qran_range(160, 196)); // Outside screen space


			// Set Enemy and apply to g_enemyArray
			g_enemyArray[i].enemyType = 0;
			g_enemyArray[i].iHealth = KNIGHT_HEALTH;
			g_enemyArray[i].iDamage = KNIGHT_DAMAGE;
			g_enemyArray[i].bAlive = true;
			g_enemyArray[i].bActive = true;
			g_enemyArray[i].iSpeed = KNIGHT_SPEED;

			// Set animation values
			a_enemy->currentFrame = 1; // Idle down
			a_enemy->animCounter = 0; // Counts up and down to animate moving animations
			a_enemy->animationSpeed = 15; // Value for counter to reach to switch frames


			g_enemyArray[i].Sprite.vSize = fv2Create(int2fix(32), int2fix(32));


			// Set attributes
			g_enemyArray[i].Sprite.Attributes.attr0 = SetAttrib0(fix2int(g_enemyArray[i].Sprite.vPosition.fY), 0, 0, 0, A0_4BPP, A0_SQUARE);
			g_enemyArray[i].Sprite.Attributes.attr1 = SetAttrib1(fix2int(g_enemyArray[i].Sprite.vPosition.fX), 0, A1_SIZE_2);
			g_enemyArray[i].Sprite.Attributes.attr2 = SetAttrib2(g_enemyArray[i].Sprite.StartTileID, 0, 4);


			MEM_OAM[g_enemyArray[i].Sprite.SpriteID] = g_enemyArray[i].Sprite.Attributes;
			break;
		}
	}


}

// Data Control
void DamageKnight(Enemy * a_enemy)
{
	a_enemy->bActive = false;
	a_enemy->bAlive = false;
	obj_hide(&a_enemy->Sprite.Attributes);

	MEM_OAM[a_enemy->Sprite.SpriteID] = a_enemy->Sprite.Attributes;
}

void UpdateKnight(Enemy * a_enemy)
{
	if (a_enemy->bActive)
	{

		fv2 direction = fv2Create(fixSub(g_PlayerObj->PlayerSprite.vPosition.fX, a_enemy->Sprite.vPosition.fX), fixSub(g_PlayerObj->PlayerSprite.vPosition.fY, a_enemy->Sprite.vPosition.fY));


		if (fix2int(direction.fX) == 0 && a_enemy->bXAxis == true) // If they've reached difference of 0 on x
		{
			a_enemy->bXAxis = false;
		}
		else if (fix2int(direction.fY) == 0 && a_enemy->bXAxis == false) // If they've reached difference of 0 on y
		{
			a_enemy->bXAxis = true;
		}


		if (a_enemy->bXAxis == true)
		{
			if (fix2int(direction.fX) > 0) // Enemy is to the left
			{
				a_enemy->Sprite.vVelocity = fv2Create(int2fix(1), 0); // Move right
				a_enemy->AnimState = MOVERIGHT;
				
				if (g_PlayerObj->AnimState == WALKRIGHT && fix2int(g_mapPos.fX) < 272) // Players moving right and enemy is moving right
				{
					a_enemy->Sprite.vVelocity = fv2Create(0, 0); // Stay still on screen
				}
				else if (g_PlayerObj->AnimState == WALKRIGHT && fix2int(g_mapPos.fX) > 272) // Players moving right and enemy is moving right
				{
					a_enemy->Sprite.vVelocity = fv2Create(int2fix(1), 0); // Stay still on screen
				}

				else if (g_PlayerObj->AnimState == RUNRIGHT && fix2int(g_mapPos.fX) < 272) // Player is running right and enemy is moving right
				{
					a_enemy->Sprite.vVelocity = fv2Create(int2fix(-1), 0); // Move back on screen a bit
				}
				else if (g_PlayerObj->AnimState == RUNRIGHT && fix2int(g_mapPos.fX) > 272) // Player is running right and enemy is moving right
				{
					a_enemy->Sprite.vVelocity = fv2Create(int2fix(1), 0); // Move back on screen a bit
				}

				else if (g_PlayerObj->AnimState == WALKLEFT && fix2int(g_mapPos.fX) > 0) // Player is moving left and enemy is moving right
				{
					a_enemy->Sprite.vVelocity = fv2Create(int2fix(2), 0); // Move enemy closer 
				}
				else if (g_PlayerObj->AnimState == WALKLEFT && fix2int(g_mapPos.fX) > 0) // Player is moving left and enemy is moving right
				{
					a_enemy->Sprite.vVelocity = fv2Create(int2fix(2), 0); // Move enemy closer 
				}
				else if (g_PlayerObj->AnimState == RUNLEFT && fix2int(g_mapPos.fX) < 0) // Player is running left and enemy is moving right 
				{
					a_enemy->Sprite.vVelocity = fv2Create(int2fix(1), 0); // Move enemy closer quicker
				}
				else if (g_PlayerObj->AnimState == RUNLEFT && fix2int(g_mapPos.fX) > 0) // Player is running left and enemy is moving right 
				{
					a_enemy->Sprite.vVelocity = fv2Create(int2fix(3), 0); // Move enemy closer quicker
				}
				else if (g_PlayerObj->AnimState == WALKUP) // Player is moving up and enemy is moving right
				{
					a_enemy->Sprite.vVelocity = fv2Create(int2fix(1), int2fix(1)); // Move the enemy on screen down and right
				}
				else if (g_PlayerObj->AnimState == RUNUP) // Player is running up and enemy is moving right
				{
					a_enemy->Sprite.vVelocity = fv2Create(int2fix(1), int2fix(2)); // Move the enemy right and up quicker
				}
				else if (g_PlayerObj->AnimState == WALKDOWN) // Player is walking down and enemy is moving right
				{
					a_enemy->Sprite.vVelocity = fv2Create(int2fix(1), int2fix(-1)); // Move enemy up and right 
				}
				else if (g_PlayerObj->AnimState == RUNDOWN) // Player is running down and enemy is moving right
				{
					a_enemy->Sprite.vVelocity = fv2Create(int2fix(1), int2fix(-2)); // Move enemy right and up quicker
				}

			}
			else if (fix2int(direction.fX) < 0) // Enemy is to the right
			{
				a_enemy->Sprite.vVelocity = fv2Create(int2fix(-1), 0); // Move left
				a_enemy->AnimState = MOVELEFT;

				if (g_PlayerObj->AnimState == WALKLEFT && fix2int(g_mapPos.fX) > 0) // Player is moving left and enemy is moving left
				{
					a_enemy->Sprite.vVelocity = fv2Create(0, 0); // enemy stay still on screen 
				}
				else if (g_PlayerObj->AnimState == RUNLEFT && fix2int(g_mapPos.fX) > 0) // Player is running left and enemy is moving left
				{
					a_enemy->Sprite.vVelocity = fv2Create(int2fix(1), 0); // Move the enemy back a little
				}
				else if (g_PlayerObj->AnimState == RUNLEFT && fix2int(g_mapPos.fX) <= 0) // Player is running left and enemy is moving left
				{
					a_enemy->Sprite.vVelocity = fv2Create(int2fix(-1), 0); // Hit edge
				}
				else if (g_PlayerObj->AnimState == WALKRIGHT && fix2int(g_mapPos.fX) < 272) // Player is moving right and enemy is moving left
				{
					a_enemy->Sprite.vVelocity = fv2Create(int2fix(-1), 0); // Move the enemy closer quicker 
				}
				else if (g_PlayerObj->AnimState == RUNRIGHT && fix2int(g_mapPos.fX) < 272) // Player is running right and 
				{
					a_enemy->Sprite.vVelocity = fv2Create(int2fix(-2), 0); // Move right
				}
				else if (g_PlayerObj->AnimState == WALKUP)
				{
					a_enemy->Sprite.vVelocity = fv2Create(int2fix(-1), int2fix(1));
				}
				else if (g_PlayerObj->AnimState == RUNUP)
				{
					a_enemy->Sprite.vVelocity = fv2Create(int2fix(-1), int2fix(2));
				}
				else if (g_PlayerObj->AnimState == WALKDOWN)
				{
					a_enemy->Sprite.vVelocity = fv2Create(int2fix(-1), int2fix(-1));
				}
				else if (g_PlayerObj->AnimState == RUNDOWN)
				{
					a_enemy->Sprite.vVelocity = fv2Create(int2fix(-1), int2fix(-2));
				}
				

			}
		}
		else if(a_enemy->bXAxis == false)
		{
			if (fix2int(direction.fY) > 0) // Enemy is above
			{
				a_enemy->Sprite.vVelocity = fv2Create(0, int2fix(1)); // Move down (y is down faced)
				a_enemy->AnimState = MOVEDOWN;

				if (g_PlayerObj->AnimState == WALKUP && fix2int(g_mapPos.fY) > 0)
				{
					a_enemy->Sprite.vVelocity = fv2Create(0, int2fix(2));
				}
				else if (g_PlayerObj->AnimState == RUNUP && fix2int(g_mapPos.fY) > 0)
				{
					a_enemy->Sprite.vVelocity = fv2Create(0, int2fix(3));
				}
				else if (g_PlayerObj->AnimState == WALKDOWN && fix2int(g_mapPos.fY) < 3936)
				{
					a_enemy->Sprite.vVelocity = fv2Create(0, 0);

				}
				else if (g_PlayerObj->AnimState == RUNDOWN && fix2int(g_mapPos.fY) < 3936)
				{
					a_enemy->Sprite.vVelocity = fv2Create(0, int2fix(-1));
				}
				else if (g_PlayerObj->AnimState == WALKRIGHT && fix2int(g_mapPos.fY) < 0)
				{
					a_enemy->Sprite.vVelocity = fv2Create(int2fix(1), int2fix(1));
				}
				else if (g_PlayerObj->AnimState == WALKRIGHT && fix2int(g_mapPos.fY) > 0)
				{
					a_enemy->Sprite.vVelocity = fv2Create(int2fix(-1), int2fix(1));
				}
				else if (g_PlayerObj->AnimState == RUNRIGHT)
				{
					a_enemy->Sprite.vVelocity = fv2Create(int2fix(-2), int2fix(1));
				}
				else if (g_PlayerObj->AnimState == WALKLEFT)
				{
					a_enemy->Sprite.vVelocity = fv2Create(int2fix(1), int2fix(1));
				}
				else if (g_PlayerObj->AnimState == RUNLEFT)
				{
					a_enemy->Sprite.vVelocity = fv2Create(int2fix(2), int2fix(1));
				}
				

			}
			else if (fix2int(direction.fY) < 0) // Enemy is below
			{
				a_enemy->Sprite.vVelocity = fv2Create(0, int2fix(-1)); // Move up
				a_enemy->AnimState = MOVEUP;
				
				if (g_PlayerObj->AnimState == WALKDOWN && fix2int(g_mapPos.fY) > 0)
				{
					a_enemy->Sprite.vVelocity = fv2Create(0, int2fix(-2));
				}
				else if (g_PlayerObj->AnimState == RUNDOWN && fix2int(g_mapPos.fY) > 0)
				{
					a_enemy->Sprite.vVelocity = fv2Create(0, int2fix(-2));
				}
				else if (g_PlayerObj->AnimState == WALKUP && fix2int(g_mapPos.fY) < 3936)
				{
					a_enemy->Sprite.vVelocity = fv2Create(0, 0);

				}
				else if (g_PlayerObj->AnimState == RUNUP && fix2int(g_mapPos.fY) < 3936)
				{
					a_enemy->Sprite.vVelocity = fv2Create(0, int2fix(2));
				}
				else if (g_PlayerObj->AnimState == WALKLEFT && fix2int(g_mapPos.fY) > 0)
				{
					a_enemy->Sprite.vVelocity = fv2Create(int2fix(1), int2fix(-1));
				}
				else if (g_PlayerObj->AnimState == WALKLEFT && fix2int(g_mapPos.fY) < 0)
				{
					a_enemy->Sprite.vVelocity = fv2Create(int2fix(0), int2fix(-1));
				}
				else if (g_PlayerObj->AnimState == RUNLEFT && fix2int(g_mapPos.fY) < 0)
				{
					a_enemy->Sprite.vVelocity = fv2Create(int2fix(0), int2fix(-1));
				}
				else if (g_PlayerObj->AnimState == RUNLEFT && fix2int(g_mapPos.fY) > 0)
				{
					a_enemy->Sprite.vVelocity = fv2Create(int2fix(2), int2fix(-1));
				}
				else if (g_PlayerObj->AnimState == WALKRIGHT && fix2int(g_mapPos.fY) < 0)
				{
					a_enemy->Sprite.vVelocity = fv2Create(int2fix(-1), int2fix(-1));

				}
				else if (g_PlayerObj->AnimState == RUNRIGHT)
				{
					a_enemy->Sprite.vVelocity = fv2Create(int2fix(-2), int2fix(-1));

				}

			}
		}

		fv2 tempSpeed = fv2Create(int2fix(a_enemy->iSpeed), int2fix(a_enemy->iSpeed));
		a_enemy->Sprite.vPosition = fvAdd(a_enemy->Sprite.vPosition, fvMul(a_enemy->Sprite.vVelocity, tempSpeed)); 


		if (fix2int(a_enemy->Sprite.vPosition.fX) > 240 && fix2int(a_enemy->Sprite.vPosition.fX) < 308)
		{
			a_enemy->Sprite.vPosition.fX = int2fix(240);
		}
		else if (fix2int(a_enemy->Sprite.vPosition.fX) < 0 && fix2int(a_enemy->Sprite.vPosition.fX) > 240)
		{
			a_enemy->Sprite.vPosition.fX = int2fix(240);
		}
		
		if (fix2int(a_enemy->Sprite.vPosition.fY) > 160 && fix2int(a_enemy->Sprite.vPosition.fY) < 228)
		{
			a_enemy->Sprite.vPosition.fY = int2fix(160);

		}
		else if (fix2int(a_enemy->Sprite.vPosition.fY) < 0 && fix2int(a_enemy->Sprite.vPosition.fY) > 160)
		{
			a_enemy->Sprite.vPosition.fY = int2fix(160);
		}

		if (AABBDynCollision(&a_enemy->Sprite, &g_PlayerObj->PlayerSprite))
		{
			DamageKnight(a_enemy);


			DamagePlayer(g_PlayerObj);


			// Hit player
		}
		else
		{ // Haven't, keep moving
			
			KnightAnimator(a_enemy);

			SetObjectvPosition(&a_enemy->Sprite.Attributes, a_enemy->Sprite.vPosition);
			MEM_OAM[a_enemy->Sprite.SpriteID] = a_enemy->Sprite.Attributes; // Apply changes to the players OAM

		}
		
	}

	if (!a_enemy->bActive)
	{
		SpawnKnight(a_enemy);
	}
}

// Animations
bool knightAnimated = false;
void KnightWalk(Enemy* a_Knight, u8 a_image1, u8 a_image2) // Flips between two images over time
{
	if (a_Knight->animCounter <= 0 && knightAnimated == true) // Counted down, switch
	{
		knightAnimated = false;
	}
	else if (a_Knight->animCounter >= a_Knight->animationSpeed && knightAnimated == false) // Counted up, switch
	{
		knightAnimated = true;
	}
	if (a_Knight->animCounter <= a_Knight->animationSpeed && knightAnimated == false) // Count up and apply current frame
	{
		a_Knight->animCounter++; // Increment
		a_Knight->currentFrame = a_image1; // Set current frame to passed frame
	}
	else if (a_Knight->animCounter >= 0 && knightAnimated == true) // Count down and apply current frame
	{
		a_Knight->animCounter--; // Decrement
		a_Knight->currentFrame = a_image2; // Set current frame to passed frame
	}
}


void KnightAnimator(Enemy *a_knight)
{

	switch (a_knight->AnimState) // Choose what frame(s) to be showing based on input passed
	{
	case MOVEDOWN:
	{
		KnightWalk(a_knight, 0, 2);
		break;
	}
	case MOVEUP:
	{
		KnightWalk(a_knight, 9, 11);
		break;
	}
	case MOVELEFT:
	{
		KnightWalk(a_knight, 3, 5);
		break;
	}
	case MOVERIGHT:
	{
		KnightWalk(a_knight, 6, 8);
		break;
	}
	default:
	{

	}
	}

	// Copy the correct tiles into player tiles
	memcpy(sprite_tile_block_address(a_knight->Sprite.StartTileID), &KnightTiles[a_knight->currentFrame * KnightFrameSize], KnightFrameLen);
}


