#include "gba/gba.h"
#include <string.h>
#include "gba/gba_math.h"
#include "gba/gba_mathUtil.h"
#include "utils/input.h"
#include "utils/sprite_manager.h"
#include "utils/animation.h"
#include "player.h"
#include "utils/map.h"


#include "gba/gba_gfx.h"

typedef struct GameData
{
	PlayerEntityData Player;
	EntityData NPCData;
}GameData;

static GameData S_GameData;

static u8 S_SELECTEDMAP = MAP_TALL_ASSET;

typedef struct {u32 data[8]; } MapRow_Half;
void FormatMapToMemory(MapData* mapData, u16 baseMapBlock)
{
	MapRow_Half* src = (MapRow_Half*)mapData->Data;

	MapRow_Half* dst = (MapRow_Half*)getBGMapBlock(baseMapBlock);

	for( u32 i = 0; i < 32; ++i)
	{
		*dst++ = *src++;   *dst++ = *src++;

		src += (mapData->Width >> 4) - 2; 
	}
}

u32 se_index_fast(u32 tx, u32 ty)
{
    u32 n= tx + ty*32;
    if(tx >= 32)
        n += 0x03E0;
    if(ty >= 32)
        n += 0x0400;
    return n;
}

void ScrollMap(MapData* mapData, u16 baseMapBlock, u32 camPosX, u32 camPosY)
{
	static u16 lastMapPosX = 0;
	static u16 lastMapPosY = 0;

	u16 mapPosX = (camPosX >> 3) & (u16)mapData->Width - 1; // / 8
	u16 mapPosY = (camPosY >> 3) & (u16)mapData->Height - 1;

	u16 screenPosX = (camPosX >> 3) & (u16)63;
	u16 screenPosY = (camPosY >> 3) & (u16)63;

	MapRow_Half* src = (MapRow_Half*)&mapData->Data[mapPosY * mapData->Width + mapPosX];

	if(mapPosY != lastMapPosY)
	{
		u16 newMapPosY = 0; u16 newScreenPosY;
		// Draw above camera
		if(mapPosY < lastMapPosY)
		{
			newMapPosY = ((camPosY >> 3) - 1) & (u16)mapData->Height - 1;
			newScreenPosY = ((camPosY >> 3) - 1) & (u16)63;
		}
		else // Draw below camera
		{
			newMapPosY = ((camPosY >> 3) + 20) & (u16)mapData->Height - 1;
			newScreenPosY = ((camPosY >> 3) + 20) & (u16)63;
		}

		MapRow_Half* dst = (MapRow_Half*) ((u16*)getBGMapBlock(baseMapBlock) + se_index_fast(screenPosX, newScreenPosY));
		MapRow_Half* src = (MapRow_Half*)&mapData->Data[newMapPosY * mapData->Width + mapPosX];
		*dst++ = *src++;   *dst++ = *src++;
	}

	// TODO: Dont do this
	if(mapPosX != lastMapPosX)
	{
	}


	lastMapPosX = mapPosX;
	lastMapPosY = mapPosY;
}


void LoadMap(MapData* map)
{
	memcpy(PAL_BG_BLOCK(0), map->LinkedPalette->Data, map->LinkedPalette->Len); // Load the bg palette into memory
	memcpy(getBGTileBlock(0), map->Tiles, map->TilesLen); // Load the tiles into memory

	FormatMapToMemory(map, 16);
	
	REG_BGCNT[0] = setBGControlRegister(0, 0, 0, 0, 16, 0, BG_REG_SIZE_64x64); // Set background control
}


void InitGameData()
{
	LoadMap(&S_Maps[S_SELECTEDMAP]);

	InitPlayer(&S_GameData.Player, 0, 0);
	//InitEntity(&S_GameData.NPCData, SCREEN_W * 0.5f, SCREEN_H * 0.5f, &S_Textures[CHARACTER_SHEET_ASSET], 0, 0);

	u8 newX = (SCREEN_W >> 1) - (S_Textures[CHARACTER_SHEET_ASSET].FrameWidth >> 1);
	u8 newY = (SCREEN_H >> 1) - (S_Textures[CHARACTER_SHEET_ASSET].FrameHeight >> 1);

	SetSpritePosition(S_GameData.Player.Entity.Sprite.ID, newX, newY);

// TODO MOVE
// =====
	memcpy(PAL_SP_BLOCK(0), S_Palettes[CHARACTER_SHEET_ASSET].Data, S_Palettes[CHARACTER_SHEET_ASSET].Len); // Copy character pallet into Pallete sprite mem
	InitUniqueAnimation(&S_GameData.Player.EntityAnim, sprite_tile_address(0), &S_Textures[CHARACTER_SHEET_ASSET], &S_Sequences[CHARACTER_SHEET_ASSET], 7);
// =====
}

fv2 playerPos;
void GameLoop()
{
	UpdatePlayer(&S_GameData.Player);

	//playerPos.X += getAxis(HORIZONTAL) * 500;
	playerPos.Y += -getAxis(VERTICAL) * 500;

	ScrollMap(&S_Maps[S_SELECTEDMAP], 16, fix2int(playerPos.X), fix2int(playerPos.Y));

	//REG_BG_OFFSET[0].x = (s16)fix2int(playerPos.X);
	REG_BG_OFFSET[0].y = (s16)fix2int(playerPos.Y);
}

int main()
{
	REG_DISPCNT = VIDEOMODE_0 | ENABLE_OBJECTS | MAPPING_MODE_1D | BGMODE_0;

	register_vblank_isr();
	oam_init(obj_buffer, 128);
	InitGameData();

	while (1)
	{
		vblank_intr_wait();
		
		PollKeys();
		UpdateInputBinds();

		GameLoop();

		oam_copy(MEM_OAM, obj_buffer, 1);
	}
	return 0;
}
