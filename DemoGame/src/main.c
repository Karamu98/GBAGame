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

void copy64x64MapIntoMemory( const u16* a_mapData, u16 a_mapBlockAddress )
{
	typedef struct {u32 data[8]; } HALF_ROW;
	//get a pointer to the map Ed data we want to convert pages for
	HALF_ROW* src = (HALF_ROW*)a_mapData;
	//a 32x32 map occupies one address space (32x16 bits no$gba demo's this well)
	//a 64x32 map occupies two address spaces, 64x64 occupies four address spaces
	HALF_ROW* dst0 = (HALF_ROW*)getBGMapBlock(a_mapBlockAddress);
	HALF_ROW* dst1 = (HALF_ROW*)getBGMapBlock(a_mapBlockAddress+1);
	HALF_ROW* dst2 = (HALF_ROW*)getBGMapBlock(a_mapBlockAddress+2);
	HALF_ROW* dst3 = (HALF_ROW*)getBGMapBlock(a_mapBlockAddress+3);
	//as there are 32 tiles per page row the following loop can be used.
	//I am taking advantage of post increment here. The assignment operator happens
	//before the increment operator
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
	//memcpy(getBGMapBlock(0), bgMap, bgMapLen); // Copy map into memory

	copy64x64MapIntoMemory(Map_TallMap_Map, 16);

	REG_BGCNT[0] = setBGControlRegister(0, 0, 0, 0, 16, 0, BG_REG_SIZE_64x64); // Set background control
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
