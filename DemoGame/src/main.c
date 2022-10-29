#include "gba/gba.h"
#include <string.h>
#include "gba/gba_math.h"
#include "utils/input.h"
#include "utils/sprite_manager.h"
#include "utils/animation.h"
#include "player.h"


#include "gba/gba_gfx.h"

#define SELECTED_MAP TallMap
#include "res/maps/TallMap/Map_TallMap.h"

typedef struct GameData
{
	PlayerEntityData Player;
	EntityData NPCData;
}GameData;

static GameData S_GameData;

typedef struct {u32 data[8]; } MapRow_Half;
void FormatMapToMemory(u16* mapData, u16 baseMapBlock)
{
	
	MapRow_Half* src = (MapRow_Half*)mapData;

	MapRow_Half* dst0 = (MapRow_Half*)getBGMapBlock(baseMapBlock);
	MapRow_Half* dst1 = (MapRow_Half*)getBGMapBlock(baseMapBlock+1);
	MapRow_Half* dst2 = (MapRow_Half*)getBGMapBlock(baseMapBlock+2);
	MapRow_Half* dst3 = (MapRow_Half*)getBGMapBlock(baseMapBlock+3);

	for( u32 i = 0; i < 32; ++i)
	{
		//copy row i of the left page
		*dst0++ = *src++;   *dst0++ = *src++;
		//copy row i of the right page
		*dst1++ = *src++;   *dst1++ = *src++;

		*dst2++ = *src++;   *dst2++ = *src++;

		*dst3++ = *src++;   *dst3++ = *src++;
	}
}


void LoadMap()
{
	memcpy(PAL_BG_BLOCK(0), Map_TallMap_Palette, Map_TallMap_PaletteLen); // Load the bg palette into memory
	memcpy(getBGTileBlock(0), Map_TallMap_Tiles, Map_TallMap_TilesLen); // Load the tiles into memory

	FormatMapToMemory(Map_TallMap_Map, 1);

	REG_BGCNT[0] = setBGControlRegister(0, 0, 0, 0, 1, 0, BG_REG_SIZE_64x64); // Set background control
}


void InitGameData()
{
	InitPlayer(&S_GameData.Player, 0, 0);
	InitEntity(&S_GameData.NPCData, SCREEN_W * 0.5f, SCREEN_H * 0.5f, &S_Textures[CHARACTER_SHEET_ASSET], 0, 0);

	u8 newX = (SCREEN_W *0.5f) - S_Textures[CHARACTER_SHEET_ASSET].FrameWidth * 0.5f;
	u8 newY = (SCREEN_H *0.5f) + S_Textures[CHARACTER_SHEET_ASSET].FrameHeight * 0.5f;

	SetSpritePosition(S_GameData.Player.Entity.Sprite.ID, newX, newY);

// TODO MOVE
// =====
	memcpy(PAL_SP_BLOCK(0), S_Palettes[CHARACTER_SHEET_ASSET].Data, S_Palettes[CHARACTER_SHEET_ASSET].Len); // Copy character pallet into Pallete sprite mem
	InitUniqueAnimation(&S_GameData.Player.EntityAnim, sprite_tile_address(0), &S_Textures[CHARACTER_SHEET_ASSET], &S_Sequences[CHARACTER_SHEET_ASSET], 7);
	LoadMap();
// =====
}

u32 mapx = 0, mapy = 0;
void GameLoop()
{
	UpdatePlayer(&S_GameData.Player);

	mapx += getAxis(HORIZONTAL);
	mapy += -getAxis(VERTICAL);

	REG_BG0HOFS = mapx;
	REG_BG0VOFS = mapy;
}

int main()
{
	REG_DISPCNT = VIDEOMODE_0 | ENABLE_OBJECTS | MAPPING_MODE_1D | BGMODE_0;

	register_vblank_isr();
	oam_init(obj_buffer, 2);
	InitGameData();

	while (1)
	{
		vblank_intr_wait();
		
		PollKeys();
		UpdateInputBinds();

		GameLoop();

		oam_copy(MEM_OAM, obj_buffer, 2);
	}
	return 0;
}
