#include "Projectile.h"
#include "Collision.h"
#include "MemoryManager.h"
#include "../images/VFireball.h"
#include "../images/HFireball.h"
#include "Enemy.h"
#include "Player.h"

#include <string.h>


void SetDamage(Projectile* a_projectile, u8 a_damage)
{
	a_projectile->iDamage = a_damage;
}

void Fire(Projectile* a_projectile, fv2 a_startPos, fv2 a_direction)
{

	a_projectile->Sprite.SpriteID = GetFreeSprite(); // Assign as first free sprite
	TagSprite(a_projectile->Sprite.SpriteID); // Notify manager

	a_projectile->Sprite.StartTileID = GetFreeTileWSpace(2); // Find first free space with two tiles in succession
	TagTile(a_projectile->Sprite.StartTileID, a_projectile->Sprite.StartTileID + 2); // Notify manager

	a_projectile->bActive = true;

	// Set projectiles velocity
	a_projectile->Sprite.vVelocity = a_direction;

	// Set the projectiles attributes based on direction
	// Create a VFlip and HFLIP function to clean these up
	if (a_projectile->Sprite.vVelocity.fX > 0 && a_projectile->Sprite.vVelocity.fY == 0) // Firing Right (Sprite original, shape wide)
	{
		a_projectile->Sprite.Attributes.attr0 = SetAttrib0(a_projectile->Sprite.vPosition.fY, 0, 0, 0, A0_4BPP, A0_WIDE);
		a_projectile->Sprite.Attributes.attr1 = SetAttrib1(a_projectile->Sprite.vPosition.fX, 0, A1_SIZE_0);
		a_projectile->Sprite.Attributes.attr2 = SetAttrib2(a_projectile->Sprite.StartTileID, 0, 1);

		memcpy(sprite_tile_block_address(a_projectile->Sprite.StartTileID), &HFireballTiles[(a_projectile->AnimationCounter >> 1) * HFireballFrameSize], HFireballFrameLen);
	}
	else if (a_projectile->Sprite.vVelocity.fY > 0 && a_projectile->Sprite.vVelocity.fX == 0) // Firing Down (Sprite flips, shape tall)
	{
		a_projectile->Sprite.Attributes.attr0 = SetAttrib0(a_projectile->Sprite.vPosition.fY, 0, 0, 0, A0_4BPP, A0_TALL);
		a_projectile->Sprite.Attributes.attr1 = SetAttrib1(a_projectile->Sprite.vPosition.fX, 2, A1_SIZE_0); // NEEDS TO BE H FLIPPED
		a_projectile->Sprite.Attributes.attr2 = SetAttrib2(a_projectile->Sprite.StartTileID, 0, 1);


		memcpy(sprite_tile_block_address(a_projectile->Sprite.StartTileID), &VFireballTiles[(a_projectile->AnimationCounter >> 1) * VFireballFrameSize], VFireballFrameLen);
	}
	else if (a_projectile->Sprite.vVelocity.fX < 0 && a_projectile->Sprite.vVelocity.fY == 0) // Firing Left (Sprite flips, shape wide)
	{
		a_projectile->Sprite.Attributes.attr0 = SetAttrib0(a_projectile->Sprite.vPosition.fY, 0, 0, 0, A0_4BPP, A0_WIDE);
		a_projectile->Sprite.Attributes.attr1 = SetAttrib1(a_projectile->Sprite.vPosition.fX, 1, A1_SIZE_0);
		a_projectile->Sprite.Attributes.attr2 = SetAttrib2(a_projectile->Sprite.StartTileID, 0, 1);

		memcpy(sprite_tile_block_address(a_projectile->Sprite.StartTileID), &HFireballTiles[(a_projectile->AnimationCounter >> 1) * HFireballFrameSize], HFireballFrameLen);

	}
	else if (a_projectile->Sprite.vVelocity.fY < 0 && a_projectile->Sprite.vVelocity.fX == 0) // Firing Up (Sprite original, shape tall)
	{
		a_projectile->Sprite.Attributes.attr0 = SetAttrib0(a_projectile->Sprite.vPosition.fY, 0, 0, 0, A0_4BPP, A0_TALL);
		a_projectile->Sprite.Attributes.attr1 = SetAttrib1(a_projectile->Sprite.vPosition.fX, 0, A1_SIZE_0);
		a_projectile->Sprite.Attributes.attr2 = SetAttrib2(a_projectile->Sprite.StartTileID, 0, 1);

		memcpy(sprite_tile_block_address(a_projectile->Sprite.StartTileID), &VFireballTiles[(a_projectile->AnimationCounter >> 1) * VFireballFrameSize], VFireballFrameLen);
	}



	obj_unhide(&a_projectile->Sprite.Attributes, A0_MODE_REG); // Unhide the sprite


	fv2 tempPos = a_startPos; // Will be manipulated to pick the location of spawn

	// Set projectiles position to activator based on direction to offset spawn

	if (a_direction.fX > 0) // If going right
	{
		tempPos.fX += int2fix(20);
		tempPos.fY += int2fix(16);
	}
	else if (a_direction.fX < 0) // If going left
	{
		tempPos.fX -= int2fix(12);
		tempPos.fY += int2fix(16);
	}
	else if (a_direction.fY > 0) // If going down
	{
		tempPos.fX += int2fix(12);
		tempPos.fY += int2fix(32);
	}
	else if (a_direction.fY < 0) // If going up
	{
		tempPos.fX += int2fix(12);
	}

	a_projectile->Sprite.vPosition = tempPos; // Set position of projectile determined by direction
	SetObjectvPosition(&a_projectile->Sprite.Attributes, a_projectile->Sprite.vPosition); // Move the sprite to it's position

	// Set emitters location to projectiles location
	a_projectile->ParticleEmitter.vPosition = a_projectile->Sprite.vPosition;

	// Apply to OAM
	MEM_OAM[a_projectile->Sprite.SpriteID] = a_projectile->Sprite.Attributes;
}

void SetProjectileSprite(Projectile* a_projectile, u16 attr0, u16 attr1, u16 attr2) // Ease of use
{
	a_projectile->Sprite.Attributes.attr0 = attr0;
	a_projectile->Sprite.Attributes.attr1 = attr1;
	a_projectile->Sprite.Attributes.attr2 = attr2;
} 

void InitialiseProjectile(Projectile* a_projectile, u8 a_damage, u16 a_speed, bool a_bHostile)
{
	// Set Projectile Vales
	a_projectile->iDamage = a_damage;
	a_projectile->iSpeed = a_speed;
	a_projectile->Sprite.vPosition = fv2Create(0, 0);
	a_projectile->bActive = false;
	a_projectile->AnimationCounter = 0;
	a_projectile->bHostile = a_bHostile;
	a_projectile->particleCount = 4;



	InitEmitter(&a_projectile->ParticleEmitter, &a_projectile->Sprite.vPosition, false, &a_projectile->Sprite.vVelocity);

	fv2 max = fv2Create(int2fix(1), int2fix(1)); // Max velocity (Direction, needs to be normalised)
	fv2 min = fv2Create(0, 0); // Min velocity (Direction, needs to be normalised)

	for (int i = 0; i < 3; i++)
	{
		InitParticle(&a_projectile->ParticleArray[i], 1, 50, 250, 500, min, max); // BUGONE
	}
}

void UpdateProjectile(Projectile* a_projectile) // Needs map and other sprites passed to
{
	if (a_projectile->bActive == true) // If projectile is active
	{
		if (a_projectile->Sprite.vPosition.fX < int2fix(270) && a_projectile->Sprite.vPosition.fY < int2fix(200) && a_projectile->Sprite.vPosition.fX > int2fix(-20) && a_projectile->Sprite.vPosition.fY >int2fix(-20)) // Test if on screen
		{
			a_projectile->AnimationCounter++; // Counts for animation

			if (a_projectile->AnimationCounter > 16) // Clamp counter to max 16
			{
				a_projectile->AnimationCounter = 0; // Resets when reached 16
			}

			if (a_projectile->AnimationCounter % 2 == 0) // If counter is dividable by 2..
			{

				// Create a VFlip and HFLIP function to clean these up
				if (a_projectile->Sprite.vVelocity.fX > 0 && a_projectile->Sprite.vVelocity.fY == 0) // Firing Right (Sprite original, shape wide)
				{
					memcpy(sprite_tile_block_address(a_projectile->Sprite.StartTileID), &HFireballTiles[(a_projectile->AnimationCounter >> 1) * HFireballFrameSize], HFireballFrameLen);
				}
				else if (a_projectile->Sprite.vVelocity.fY > 0 && a_projectile->Sprite.vVelocity.fX == 0) // Firing Down (Sprite flips, shape tall)
				{
					memcpy(sprite_tile_block_address(a_projectile->Sprite.StartTileID), &VFireballTiles[(a_projectile->AnimationCounter >> 1) * VFireballFrameSize], VFireballFrameLen);
				}
				else if (a_projectile->Sprite.vVelocity.fX < 0 && a_projectile->Sprite.vVelocity.fY == 0) // Firing Left (Sprite flips, shape wide)
				{
					memcpy(sprite_tile_block_address(a_projectile->Sprite.StartTileID), &HFireballTiles[(a_projectile->AnimationCounter >> 1) * HFireballFrameSize], HFireballFrameLen);
				}
				else if (a_projectile->Sprite.vVelocity.fY < 0 && a_projectile->Sprite.vVelocity.fX == 0) // Firing Up (Sprite original, shape tall)
				{
					memcpy(sprite_tile_block_address(a_projectile->Sprite.StartTileID), &VFireballTiles[(a_projectile->AnimationCounter >> 1) * VFireballFrameSize], VFireballFrameLen);
				}
			}



			// Set sprites new position based on velocity
			fv2 temp = fv2Create(a_projectile->iSpeed, a_projectile->iSpeed); // Stores speed in fv2 (Need a cleaner method)
			a_projectile->Sprite.vPosition = fvAdd(a_projectile->Sprite.vPosition, fvMul(a_projectile->Sprite.vVelocity, temp));

			// Test for collisions
			if (a_projectile->bHostile)
			{
				if (AABBDynCollision(&a_projectile->Sprite, &g_PlayerObj->PlayerSprite))
				{
					a_projectile->bActive = false;;
					DropSprite(a_projectile->Sprite.SpriteID);
					DropTile(a_projectile->Sprite.StartTileID, a_projectile->Sprite.StartTileID + 2);
					obj_hide(&a_projectile->Sprite.Attributes);
					
					a_projectile->ParticleEmitter.vPosition = a_projectile->Sprite.vPosition;
					for (int i = 0; i < a_projectile->particleCount; i++)
					{
						EmitParticle(&a_projectile->ParticleEmitter, &a_projectile->ParticleArray[i]);
					}
				}
				else
				{
					// Move sprite if no collisions
					SetObjectvPosition(&a_projectile->Sprite.Attributes, a_projectile->Sprite.vPosition);
				}
			}
			else
			{
				for (int i = 0; i < MAX_ENEMIES; i++)
				{
					if (AABBDynCollision(&a_projectile->Sprite, &g_enemyArray[i].Sprite) && g_enemyArray[i].bActive && g_enemyArray[i].bAlive)
					{
						DropSprite(a_projectile->Sprite.SpriteID);
						DropTile(a_projectile->Sprite.StartTileID, a_projectile->Sprite.StartTileID + 2);
						obj_hide(&a_projectile->Sprite.Attributes);
						a_projectile->bActive = false;

						DamageKnight(&g_enemyArray[i]);



						a_projectile->ParticleEmitter.vPosition = a_projectile->Sprite.vPosition;
						for (int i = 0; i < a_projectile->particleCount; i++)
						{
							EmitParticle(&a_projectile->ParticleEmitter, &a_projectile->ParticleArray[i]);

						}
						break;
					}
					else
					{
						// Move sprite if no collisions
						SetObjectvPosition(&a_projectile->Sprite.Attributes, a_projectile->Sprite.vPosition);
					}
				}
			}



			// Move sprite if no collisions
			SetObjectvPosition(&a_projectile->Sprite.Attributes, a_projectile->Sprite.vPosition);

			// Apply new position to sprite OAM
			MEM_OAM[a_projectile->Sprite.SpriteID] = a_projectile->Sprite.Attributes;
		} 
		else // Projectile off screen
		{
			a_projectile->bActive = false;
			obj_hide(&a_projectile->Sprite.Attributes); // Hide the projectile

			// Apply too OAM
			MEM_OAM[a_projectile->Sprite.SpriteID] = a_projectile->Sprite.Attributes;


			DropSprite(a_projectile->Sprite.SpriteID); // Free up the sprite space
			DropTile(a_projectile->Sprite.StartTileID, a_projectile->Sprite.StartTileID + 2); // Free up tile space
		}
	}
	else // Projectile is inactive
	{

	}

	for (int i = 0; i < a_projectile->particleCount; i++)
	{
		UpdateParticle(&a_projectile->ParticleArray[i]);
	}
}


