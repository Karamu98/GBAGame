#include "Player.h"
#include "CharacterSheet.h"
#include "MemoryManager.h"

#include <string.h>

void InitialisePlayer(Player *a_player)
{
	// Set player values
	a_player->iHealth = 4;
	a_player->iProjectileCooldown = 20;
	a_player->iProjectileCounter = 0;
	a_player->iSpeed = 1;
	a_player->bulletCount = 4;
	a_player->PlayerSprite.vPosition = fv2Create(int2fix(104), int2fix(64)); // (Screenwidth / 2) - (spritesize / 2)
	a_player->PlayerSprite.vSize = fv2Create(int2fix(32), int2fix(32));

	// Setting Memory ID's
	a_player->PlayerSprite.SpriteID = GetFreeSprite(); // Get next free sprite
	TagSprite(a_player->PlayerSprite.SpriteID); // Notify manager that the sprite is in use
	
	a_player->PlayerSprite.StartTileID = GetFreeTileWSpace(16); // Get next tile space with 16 tiles following in succession
	TagTile(a_player->PlayerSprite.StartTileID, (a_player->PlayerSprite.StartTileID + 16)); // Notify that these tiles are now in use


	// Set animation values
	a_player->currentFrame = 1; // Idle down
	a_player->animCounter = 0; // Counts up and down to animate moving animations
	a_player->animationSpeed = 15; // Value for counter to reach to switch frames
	a_player->AnimState = IDLEDOWN; // First state to be in



	// Initialise the player sprite
	SetPlayerSprite
	(
		a_player,
		SetAttrib0(fix2int(a_player->PlayerSprite.vPosition.fY), 0, 0, 0, A0_4BPP, A0_SQUARE),
		SetAttrib1(fix2int(a_player->PlayerSprite.vPosition.fX), 0, A1_SIZE_2), 
		SetAttrib2(a_player->PlayerSprite.StartTileID, 0, 0)
	);

	// Initialise the player bullets
	for (int i = 0; i < a_player->bulletCount; i++)
	{
		InitialiseProjectile(&a_player->bullets[i], 5, 500, false); // Intialise projectiles with 5 damage and 750 speed
	}

	// Apply to player OAM
	MEM_OAM[a_player->PlayerSprite.SpriteID] = a_player->PlayerSprite.Attributes;
}

// Animations
bool animated = false; // Used only for SpriteWalk and SpriteRun function (Should be apart of the player)
void SpriteWalk(Player* a_player, u8 a_image1, u8 a_image2) // Flips between two images over time
{
	if (a_player->animCounter <= 0 && animated == true) // Counted down, switch
	{
		animated = false;
	}
	else if (a_player->animCounter >= a_player->animationSpeed && animated == false) // Counted up, switch
	{
		animated = true;
	}
	if (a_player->animCounter <= a_player->animationSpeed && animated == false) // Count up and apply current frame
	{
		a_player->animCounter++; // Increment
		a_player->currentFrame = a_image1; // Set current frame to passed frame
	}
	else if (a_player->animCounter >= 0 && animated == true) // Count down and apply current frame
	{
		a_player->animCounter--; // Decrement
		a_player->currentFrame = a_image2; // Set current frame to passed frame
	}
}

void SpriteRun(Player* a_player, u8 a_image1, u8 a_image2) // Same as SpriteWalk but counts faster
{
	if (a_player->animCounter <= 0 && animated == true)
	{
		animated = false;
	}
	else if (a_player->animCounter >= a_player->animationSpeed && animated == false)
	{
		animated = true;
	}
	if (a_player->animCounter <= a_player->animationSpeed && animated == false)
	{
		a_player->animCounter++;
		a_player->animCounter++; // Count again
		a_player->currentFrame = a_image1;
	}
	else if (a_player->animCounter >= 0 && animated == true)
	{
		a_player->animCounter--;
		a_player->animCounter--; // Count again
		a_player->currentFrame = a_image2;
	}
}

void PlayerAnimator(Player *a_player)
{

	switch (a_player->AnimState) // Choose what frame(s) to be showing based on input passed
	{
	case IDLEDOWN:
	{
		a_player->currentFrame = 1;
		break;
	}
	case IDLEUP:
	{
		a_player->currentFrame = 10;
		break;
	}
	case IDLELEFT:
	{
		a_player->currentFrame = 4;
		break;
	}
	case IDLERIGHT:
	{
		a_player->currentFrame = 7;
		break;
	}
	case WALKDOWN:
	{
		SpriteWalk(a_player, 0, 2);
		break;
	}
	case WALKUP:
	{
		SpriteWalk(a_player, 9, 11);
		break;
	}
	case WALKLEFT:
	{
		SpriteWalk(a_player, 3, 5);
		break;
	}
	case WALKRIGHT:
	{
		SpriteWalk(a_player, 6, 8);
		break;
	}
	case RUNDOWN:
	{
		SpriteRun(a_player, 0, 2);
		break;
	}
	case RUNUP:
	{
		SpriteRun(a_player, 9, 11);
		break;
	}
	case RUNLEFT:
	{
		SpriteRun(a_player, 3, 5);
		break;
	}
	case RUNRIGHT:
	{
		SpriteRun(a_player, 6, 8);
		break;
	}
	default:
		break;
	}

	// Copy the correct tiles into player tiles
	memcpy(sprite_tile_block_address(a_player->PlayerSprite.StartTileID), &CharacterSheetTiles[a_player->currentFrame * CharacterFrameSize], CharacterFrameLen);
}

void PlayerShoot(Player* a_player, fv2 a_direction)
{
	if (a_player->iProjectileCounter <= 0) // If the player's cool down has ended...
	{
		for (int i = 0; i < a_player->bulletCount; i++) // Loop through player projectiles..
		{
			if (!a_player->bullets[i].bActive) // Find first inactive..
			{
				Fire(&a_player->bullets[i], a_player->PlayerSprite.vPosition, a_direction); // Fire this projectile
				a_player->bullets[i].bHostile = false;
				a_player->iProjectileCounter = a_player->iProjectileCooldown; // Reset the cooldown to count down again
				break; // Don't need to fire more than one, break
			}
		}
	}
}


void HandleGameInput(Player *a_player)
{
	if (a_player->iHealth > 0)
	{
		if (a_player->iProjectileCounter > 0) // If the player's cool down hasnt reached the end...
		{
			a_player->iProjectileCounter--; // Decreases timer to allow player to shoot
		}

		// ========================================================= Start of velocity/animator setting and shooting

		fv2 Vel; // Temp value stores new velocity based on input

				 // ======= DOWN =======
		if (keyDown(DOWN) && keyDown(R))
		{
			a_player->AnimState = RUNDOWN; // Sets the animator
			Vel = fv2Create(0, int2fix(2)); // Sets the temp value to the correct velocity
			a_player->PlayerSprite.vVelocity = Vel; // Assigns to player
		}
		else if (keyDown(DOWN) && !keyDown(R))
		{
			a_player->AnimState = WALKDOWN; // Sets the animator
			Vel = fv2Create(0, int2fix(1)); // Sets the temp value to the correct velocity
			a_player->PlayerSprite.vVelocity = Vel; // Assigns to player

		}
		if (keyReleased(DOWN))
		{
			a_player->AnimState = IDLEDOWN; // Change animator state
		}


		// ======= UP =======
		if (keyDown(UP) && keyDown(R))
		{
			a_player->AnimState = RUNUP; // Sets the animator
			Vel = fv2Create(0, int2fix(-2)); // Sets the temp value to the correct velocity
			a_player->PlayerSprite.vVelocity = Vel; // Assigns to player
		}
		else if (keyDown(UP) && !keyDown(R))
		{
			a_player->AnimState = WALKUP; // Sets the animator
			Vel = fv2Create(0, int2fix(-1)); // Sets the temp value to the correct velocity
			a_player->PlayerSprite.vVelocity = Vel; // Assigns to player
		}
		if (keyReleased(UP))
		{
			a_player->AnimState = IDLEUP; // Change animator state
		}


		// ======= LEFT =======
		if (keyDown(LEFT) && keyDown(R))
		{
			a_player->AnimState = RUNLEFT; // Sets the animator
			Vel = fv2Create(int2fix(-2), 0); // Sets the temp value to the correct velocity
			a_player->PlayerSprite.vVelocity = Vel; // Assigns to player
		}
		else if (keyDown(LEFT) && !keyDown(R))
		{
			a_player->AnimState = WALKLEFT; // Sets the animator
			Vel = fv2Create(int2fix(-1), 0); // Sets the temp value to the correct velocity
			a_player->PlayerSprite.vVelocity = Vel; // Assigns to player
		}
		if (keyReleased(LEFT))
		{
			a_player->AnimState = IDLELEFT; // Change animator state
		}


		// ======= RIGHT =======
		if (keyDown(RIGHT) && keyDown(R))
		{
			a_player->AnimState = RUNRIGHT; // Sets the animator
			Vel = fv2Create(int2fix(2), 0); // Sets the temp value to the correct velocity
			a_player->PlayerSprite.vVelocity = Vel; // Assigns to player
		}
		else if (keyDown(RIGHT) && !keyDown(R))
		{
			a_player->AnimState = WALKRIGHT; // Sets the animator
			Vel = fv2Create(int2fix(1), 0); // Sets the temp value to the correct velocity
			a_player->PlayerSprite.vVelocity = Vel; // Assigns to player
		}
		if (keyReleased(RIGHT))
		{
			a_player->AnimState = IDLERIGHT; // Change animator state
		}


		// ======= SHOOTING =======
		if (keyDown(A))
		{
			switch (a_player->AnimState)
			{
			case IDLEDOWN:
			{
				PlayerShoot(a_player, fv2Create(0, int2fix(1)));
				break;
			}
			case IDLEUP:
			{
				PlayerShoot(a_player, fv2Create(0, int2fix(-1)));
				break;
			}
			case IDLELEFT:
			{
				PlayerShoot(a_player, fv2Create(int2fix(-1), 0));
				break;
			}
			case IDLERIGHT:
			{
				PlayerShoot(a_player, fv2Create(int2fix(1), 0));
				break;
			}
			case WALKDOWN:
			{
				PlayerShoot(a_player, fv2Create(0, int2fix(1)));
				break;
			}
			case WALKUP:
			{
				PlayerShoot(a_player, fv2Create(0, int2fix(-1)));
				break;
			}
			case WALKLEFT:
			{
				PlayerShoot(a_player, fv2Create(int2fix(-1), 0));
				break;
			}
			case WALKRIGHT:
			{
				PlayerShoot(a_player, fv2Create(int2fix(1), 0));
				break;
			}
			case RUNDOWN:
			{
				break;
			}
			case RUNUP:
			{
				break;
			}
			case RUNLEFT:
			{
				break;
			}
			case RUNRIGHT:
			{
				break;
			}
			}
		}

		// ========================================================= End of velocity/animator setting and shooting


		// ========================================================= Start of "camera" control

		fv2 tempPos = a_player->PlayerSprite.vPosition; // Holds the value of the players new position if passes tests
		fv2 tempSpeed = fv2Create(int2fix(a_player->iSpeed), int2fix(a_player->iSpeed)); // Stores speed in a fv2 (Needs a cleaner method)


		// Temp move the player to test later
		// 

		// Hit top left, move player sprite on x and Y
		if (fix2int(g_mapPos.fX) <= 0 && fix2int(g_mapPos.fY) <= 0)
		{
			tempPos = fvAdd(a_player->PlayerSprite.vPosition, fvMul(a_player->PlayerSprite.vVelocity, tempSpeed)); // Move the player on any axis
		}

		// Hit top right, move player sprite on x and Y
		else if (fix2int(g_mapPos.fX) >= 272 && fix2int(g_mapPos.fY) <= 0)
		{
			tempPos = fvAdd(a_player->PlayerSprite.vPosition, fvMul(a_player->PlayerSprite.vVelocity, tempSpeed)); // Move the player on any axis
		}

		// Hit bottom left, move player sprite on x and Y
		else if (fix2int(g_mapPos.fX) <= 0 && fix2int(g_mapPos.fY) >= 3936)
		{
			tempPos = fvAdd(a_player->PlayerSprite.vPosition, fvMul(a_player->PlayerSprite.vVelocity, tempSpeed)); // Move the player on any axis
		}

		// Hit bottom right, move player sprite on x and Y
		else if (fix2int(g_mapPos.fX) >= 272 && fix2int(g_mapPos.fY) >= 3936)
		{
			tempPos = fvAdd(a_player->PlayerSprite.vPosition, fvMul(a_player->PlayerSprite.vVelocity, tempSpeed)); // Move the player on any axis
		}

		// Hit left, move player sprite only on X
		else if (fix2int(g_mapPos.fX) <= 0 && fix2int(g_mapPos.fY) > 0 && fix2int(g_mapPos.fY) < 3936)
		{
			tempPos.fX = fixAdd(a_player->PlayerSprite.vPosition.fX, fixMul(a_player->PlayerSprite.vVelocity.fX, int2fix(a_player->iSpeed))); // Move player on y axis

			REG_BG_OFS[0].y = (s16)fix2int(g_mapPos.fY);

		}

		// Hit right, move player sprite only on X
		else if (fix2int(g_mapPos.fX) >= 272 && fix2int(g_mapPos.fY) < 3936 && fix2int(g_mapPos.fY) > 0)
		{
			tempPos.fX = fixAdd(a_player->PlayerSprite.vPosition.fX, fixMul(a_player->PlayerSprite.vVelocity.fX, int2fix(a_player->iSpeed))); // Move player on y axis

			REG_BG_OFS[0].y = (s16)fix2int(g_mapPos.fY);

		}

		// Hit Top, move player sprite only on Y
		else if (fix2int(g_mapPos.fY) <= 0 && fix2int(g_mapPos.fX) > 0 && fix2int(g_mapPos.fX) < 272)
		{
			tempPos.fY = fixAdd(a_player->PlayerSprite.vPosition.fY, fixMul(a_player->PlayerSprite.vVelocity.fY, int2fix(a_player->iSpeed))); // Move player on y axis

			REG_BG_OFS[0].x = (s16)fix2int(g_mapPos.fX);

		}

		// Hit Bottom, move player sprite only on Y
		else if (fix2int(g_mapPos.fY) >= 3936 && fix2int(g_mapPos.fX) < 272 && fix2int(g_mapPos.fX) > 0)
		{
			tempPos.fY = fixAdd(a_player->PlayerSprite.vPosition.fY, fixMul(a_player->PlayerSprite.vVelocity.fY, int2fix(a_player->iSpeed))); // Move player on y axis

			REG_BG_OFS[0].x = (s16)fix2int(g_mapPos.fX);

		}

		else if (fix2int(g_mapPos.fX) > 0 && fix2int(g_mapPos.fY) > 0 && fix2int(g_mapPos.fX) < 272 && fix2int(g_mapPos.fY) < 3936)
		{
			tempPos = fv2Create(int2fix(104), int2fix(64));

			REG_BG_OFS[0].x = (s16)fix2int(g_mapPos.fX);
			REG_BG_OFS[0].y = (s16)fix2int(g_mapPos.fY);
		}


		// Test and apply movement to the players sprite if avalible
		if (fix2int(tempPos.fX) >= 32 && fix2int(tempPos.fY) >= 32 && fix2int(tempPos.fX) <= 176 && fix2int(tempPos.fY) <= 96) // Test that new pos is okay (Screen width and height - sprite w/h)
		{
			a_player->PlayerSprite.vPosition = tempPos; // Apply player pos to passed pos

			SetObjectvPosition(&a_player->PlayerSprite.Attributes, a_player->PlayerSprite.vPosition); // Changes sprites x and y
			g_mapPos = fvAdd(g_mapPos, fvMul(a_player->PlayerSprite.vVelocity, tempSpeed)); // Camera point always moves unless player cant either. This isnt the actual background

		}

		// Test players map position (Not screen) again tiles/enemies for collision and take appropiate action



		// ========================================================= End of "camera" control


		PlayerAnimator(a_player); // Apply animation to the player
		MEM_OAM[a_player->PlayerSprite.SpriteID] = a_player->PlayerSprite.Attributes; // Apply changes to the players OAM

		a_player->vLastVel = a_player->PlayerSprite.vVelocity;
		a_player->PlayerSprite.vVelocity = fv2Create(0, 0); // Resets the player velocity




		// Updates players Projectiles
		for (int i = 0; i < a_player->bulletCount; i++)
		{
			UpdateProjectile(&a_player->bullets[i]);
		}
	}
	else // Player is dead
	{

	}
}


void SetPlayerSprite(Player *a_player, u16 attr0, u16 attr1, u16 attr2) // Ease of use
{
	a_player->PlayerSprite.Attributes.attr0 = attr0;
	a_player->PlayerSprite.Attributes.attr1 = attr1;
	a_player->PlayerSprite.Attributes.attr2 = attr2;
}

void DamagePlayer(Player *a_player)
{
	a_player->iHealth--;
}
