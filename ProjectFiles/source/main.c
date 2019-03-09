#include "Intellisense.h"
#include <string.h>


// Images
#include "../images/CharacterSheet.h" // Player Character sprite sheet
#include "../images/EmberParticle.h" // Ember sprite sheet
#include "../images/Knight.h" // Knight enemy sprite sheet
#include "../images/Mage.h" // Mage enemy sprite sheet
#include "../images/VFireball.h" // Fireball verticle sprite sheet
#include "../images/HFireball.h" // Fireball horizontal sprite sheet
#include "../images/BG_Externs.h" // Level1 Map data
#include "../images/TitleScreen.h" // Title screen image


// Headers
#include "gba.h"
#include "Player.h"
#include "Enemy.h"
#include "Collision.h"
#include "MemoryManager.h"

/* 
=========== Notes ===========

_	Questions/Bugs	_

	BUGTWO - Only 7th and 8th bit are reset for tilemanager
	BUGTHREE - EnemyAI needs to be reworked
*/

// Prototypes
void InitMain(); // Runs all initialisations for the system
void LoadMap(); // Loads initial portion of visible map
void MoveMap(AnimationState* a_anim); // Moves the map based on the player pos
// \Prototypes


// Globals
fv2 g_mapPos = {0, 512}; // Stores the players "viewport"
u16* g_mapLocPointer = (u16*)bgMap; // Pointer to map tile data (Probably could be deleted, doesn't benefit from being stored)

Player* g_PlayerObj; // Create the player

Enemy* g_enemyArray = NULL; // Stores all enemy structs for Knights and mages
Enemy enemyArray[MAX_ENEMIES];
// \Globals

typedef enum GameState
{
	MENU,
	GAMEPLAY,
	LOSE,
	COMPLETE
}GameState;


int main()
{
	GameState STATE = MENU;
	GameState PREV = STATE;
	bool ini = false;


	while (1)
	{
		if (STATE != PREV)
		{
			PREV = STATE;
			ini = false;
		}


		switch (STATE)
		{
		case MENU:
		{
			if (ini == false) // Run once
			{
				REG_DISPLAYCONTROL = VIDEOMODE_3 | BGMODE_2;

				for (int i = 0; i < 38400; i++)
				{
					SCREENBUFFER[i] = TitleScreenBitmap[i];
				}

			}

			PollKeys();

			if (keyDown(A))
			{
				STATE = GAMEPLAY;
			}


			break;
		}

		case GAMEPLAY:
		{

			// =============== Game data and Initialisation ===============

			Player PlayerObj;
			g_PlayerObj = &PlayerObj;

			if (ini == false)
			{
				InitMain(); // Set up one times (BG, palette loading etc)
				g_enemyArray = enemyArray;
				InitialisePlayer(&PlayerObj); // Initialise the player
				InitialiseEnemies();


			}

			// =============== Game Loop ===============
			while (STATE == GAMEPLAY)
			{
				if (g_PlayerObj->iHealth <= 0)
				{
					STATE = MENU;
				}

				vsync(); // Stops screen tearing
				PollKeys(); // Polls the keys to use for input



				HandleGameInput(&PlayerObj); // Handles all player data and their input



				for (int i = 0; i < MAX_ENEMIES; i++)
				{
					UpdateKnight(&enemyArray[i]);
				}



				MoveMap(&PlayerObj.AnimState);

			}
			break;
		}

		case LOSE:
		{
			STATE = MENU;
			break;
		}
		case COMPLETE:
		{
			break;
		}
		default:
			break;
		}
	}

	return 0; 
}



void InitMain()
{
	REG_DISPLAYCONTROL = VIDEOMODE_0 | ENABLE_OBJECTS | MAPPINGMODE_1D | BGMODE_0; // Set rendering modes

	oam_init(obj_buffer, 128); // Initiates all sprites (Hide and zero)
	InitManager(); // Initiates the sprite/tile manager (Sets all false)

	// Copy the pallets into pallet memory
	memcpy(palette_sp_block_address(0), CharacterSheetPal, CharacterSheetPalLen); // Player pallet
	memcpy(palette_sp_block_address(1), VFireballPal, VFireballPalLen); // Fireball verticle pallet
	memcpy(palette_sp_block_address(2), HFireballPal, HFireballPalLen); // Fireball horizontal pallet (This and above could be merged, no colour differences)
	memcpy(palette_sp_block_address(3), EmberParticlePal, EmberParticlePalLen); // Ember particles pallet
	memcpy(palette_sp_block_address(4), KnightPal, KnightPalLen); // Knights Pallet

	// Store particle tiles in the last tile block as these will be used for any particles on screen (Optimisation as issues of too many tiles being used)
	memcpy(sprite_tile_block_address(1020), &EmberParticleTiles[EmberFrameSize], EmberFrameLen);


	LoadMap(); // Loads the first portion of the map
}

void LoadMap()
{
	memcpy(palette_bg_block_address(0), bgPalette, bgPalLen); // Load the bg palette into memory

	memcpy(tile_block_address(16), bgTiles, bgTilesLen); // Load the tiles into memory

	u16* tilemapMemLoc = tileMap_block_address(16); // Copy from this address
	g_mapLocPointer = (u16*)bgMap;

	for (int i = 0; i < 128; ++i)
	{
		memcpy(tilemapMemLoc, g_mapLocPointer, 64);
		tilemapMemLoc = tilemapMemLoc + 32;
		g_mapLocPointer = g_mapLocPointer + 64;
		if (i == 31)
		{
			g_mapLocPointer = (u16*)bgMap + 32;
		}
		else if (i == 63)
		{
			g_mapLocPointer = (u16*)bgMap + 2048;
		}
		else if (i == 95)
		{
			g_mapLocPointer = (u16*)bgMap + 2048 + 32;
		}

	}
	
	REG_BGCNT[0] = setBG_Control_Register(0, 0, 0, 0, 16, 0, BG_REG_SIZE_64x64); // Set background control
}

void MoveMap(AnimationState* a_anim) // Moves the map based on the player pos
{
	u16* tilemapMemLoc = tileMap_block_address(16); // Back to tile 0, 0

	g_mapLocPointer = (u16*)bgMap; // Reset the pointer

	
	s32 xPos; // Viewpoint anchor x
	s32 yPos; // Viewpoint anchor y
	

	// Clamping values between map values so when the player reaches an edge they dont load the other side of the map
	// X Position
	if (g_mapPos.fX <= 0)
	{
		xPos = 0; 
	} 
	else if (fix2int(g_mapPos.fX) >= 272)
	{
		xPos = 272;
	}
	else
	{
		xPos = fix2int(g_mapPos.fX);
	}
	
	// Y Position
	if (g_mapPos.fY <= 0)
	{
		yPos = 0;

	}
	else if (fix2int(g_mapPos.fY) >= 3936)
	{
		yPos = 3936;
	}
	else
	{
		yPos = fix2int(g_mapPos.fY);
	}
	

	// Based on what direction the player is moving, load in a portion of the map in the correct location
	switch (*(a_anim))
	{
	case WALKDOWN: // Moving Down
	{
		s32 xTilePos = (xPos >> 3) & 0x3F; // Get current map position and AND with 63 so it wraps back around to 0 if over (Convert map position to map tile)
		s32 yTilePos = ((yPos >> 3) + 20) & 0x1FF; // Get current map position and AND with 511 so it wraps back around to 0 if over (Convert map position to map tile)

		s32 xScreenTile = (xPos >> 3) & 0x3F; // Get map tile number and AND with 63 so it wraps back around to 0 if over (Convert Map tile to screen tile)
		s32 yScreenTile = ((yPos >> 3) + 20) & 0x3F; // Get map tile number and AND with 63 so it wraps back around to 0 if over (Convert Map tile to screen tile)

		// g_mapLocPointer needs to be set to screen x and y
		// tilemapMemLoc needs to be set to tile x and y pos

		u16* mapAddr = &g_mapLocPointer[(yTilePos << 6) + (xTilePos - xScreenTile)]; // Tile y * 512(Map width) + tile 

		u16* tileAddr = &tilemapMemLoc[(yScreenTile << 5)]; // Screen y * 32(bgblock width)



		// 0 - 31 1st block 32 - 63 second on y axis

		if (yScreenTile >= 0 && yScreenTile < 32) // First block
		{
			memcpy(tileAddr, mapAddr, 64);
			tileAddr += 1024;// Next block same row
			mapAddr += 32; // next row
			memcpy(tileAddr, mapAddr, 64);
		}

		if (yScreenTile >= 32 && yScreenTile < 64)
		{
			tileAddr += 1024;
			memcpy(tileAddr, mapAddr, 64);
			tileAddr += 1024; // Goto next block same row
			mapAddr += 32;
			memcpy(tileAddr, mapAddr, 64);
		}


		break;
	}
	case RUNDOWN: // Moving Down
	{
		s32 xTilePos = (xPos >> 3) & 0x3F; // Get current map position and AND with 63 so it wraps back around to 0 if over (Convert map position to map tile)
		s32 yTilePos = ((yPos >> 3) + 20) & 0x1FF; // Get current map position and AND with 511 so it wraps back around to 0 if over (Convert map position to map tile)

		s32 xScreenTile = (xPos >> 3) & 0x3F; // Get map tile number and AND with 63 so it wraps back around to 0 if over (Convert Map tile to screen tile)
		s32 yScreenTile = ((yPos >> 3) + 20) & 0x3F; // Get map tile number and AND with 63 so it wraps back around to 0 if over (Convert Map tile to screen tile)

													 // g_mapLocPointer needs to be set to screen x and y
													 // tilemapMemLoc needs to be set to tile x and y pos

		u16* mapAddr = &g_mapLocPointer[(yTilePos << 6) + (xTilePos - xScreenTile)]; // Tile y * 512(Map width) + tile 

		u16* tileAddr = &tilemapMemLoc[(yScreenTile << 5)]; // Screen y * 32(bgblock width)



															// 0 - 31 1st block 32 - 63 second on y axis

		if (yScreenTile >= 0 && yScreenTile < 32) // First block
		{
			memcpy(tileAddr, mapAddr, 64);
			tileAddr += 1024;// Next block same row
			mapAddr += 32; // next row
			memcpy(tileAddr, mapAddr, 64);
		}

		if (yScreenTile >= 32 && yScreenTile < 64)
		{
			tileAddr += 1024;
			memcpy(tileAddr, mapAddr, 64);
			tileAddr += 1024; // Goto next block same row
			mapAddr += 32;
			memcpy(tileAddr, mapAddr, 64);
		}


		break;
	}
	case WALKUP: // Moving Down
	{
		s32 xTilePos = (xPos >> 3) & 0x3F; // Get current map position and AND with 63 so it wraps back around to 0 if over (Convert map position to map tile)
		s32 yTilePos = ((yPos >> 3) - 1) & 0x1FF; // Get current map position and AND with 511 so it wraps back around to 0 if over (Convert map position to map tile)

		s32 xScreenTile = (xPos >> 3) & 0x3F; // Get map tile number and AND with 63 so it wraps back around to 0 if over (Convert Map tile to screen tile)
		s32 yScreenTile = ((yPos >> 3) - 1) & 0x3F; // Get map tile number and AND with 63 so it wraps back around to 0 if over (Convert Map tile to screen tile)

													 // g_mapLocPointer needs to be set to screen x and y
													 // tilemapMemLoc needs to be set to tile x and y pos

		u16* mapAddr = &g_mapLocPointer[(yTilePos << 6) + (xTilePos - xScreenTile)]; // Tile y * 512(Map width) + tile 

		u16* tileAddr = &tilemapMemLoc[(yScreenTile << 5)]; // Screen y * 32(bgblock width)



															// 0 - 31 1st block 32 - 63 second on y axis

		if (yScreenTile >= 0 && yScreenTile < 32) // First block
		{
			memcpy(tileAddr, mapAddr, 64);
			tileAddr += 1024;// Next block same row
			mapAddr += 32; // next row
			memcpy(tileAddr, mapAddr, 64);
		}

		if (yScreenTile >= 32 && yScreenTile < 64)
		{
			tileAddr += 1024;
			memcpy(tileAddr, mapAddr, 64);
			tileAddr += 1024; // Goto next block same row
			mapAddr += 32;
			memcpy(tileAddr, mapAddr, 64);
		}


		break;
	}
	case RUNUP: // Moving Up
	{
		s32 xTilePos = (xPos >> 3) & 0x3F; // Get current map position and AND with 63 so it wraps back around to 0 if over (Convert map position to map tile)
		s32 yTilePos = ((yPos >> 3) - 1) & 0x1FF; // Get current map position and AND with 511 so it wraps back around to 0 if over (Convert map position to map tile)

		s32 xScreenTile = (xPos >> 3) & 0x3F; // Get map tile number and AND with 63 so it wraps back around to 0 if over (Convert Map tile to screen tile)
		s32 yScreenTile = ((yPos >> 3) - 1) & 0x3F; // Get map tile number and AND with 63 so it wraps back around to 0 if over (Convert Map tile to screen tile)

													// g_mapLocPointer needs to be set to screen x and y
													// tilemapMemLoc needs to be set to tile x and y pos

		u16* mapAddr = &g_mapLocPointer[(yTilePos << 6) + (xTilePos - xScreenTile)]; // Tile y * 512(Map width) + tile 

		u16* tileAddr = &tilemapMemLoc[(yScreenTile << 5)]; // Screen y * 32(bgblock width)



															// 0 - 31 1st block 32 - 63 second on y axis

		if (yScreenTile >= 0 && yScreenTile < 32) // First block
		{
			memcpy(tileAddr, mapAddr, 64);
			tileAddr += 1024;// Next block same row
			mapAddr += 32; // next row
			memcpy(tileAddr, mapAddr, 64);
		}

		if (yScreenTile >= 32 && yScreenTile < 64)
		{
			tileAddr += 1024;
			memcpy(tileAddr, mapAddr, 64);
			tileAddr += 1024; // Goto next block same row
			mapAddr += 32;
			memcpy(tileAddr, mapAddr, 64);
		}


		break;
	}

	}
}