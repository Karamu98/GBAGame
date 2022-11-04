#include "map.h"
#include <string.h>

#include "../gba/gba_gfx.h"
#include "palette.h"

#include "../res/maps/TallMap/Map_TallMap.h"
#include "../res/maps/BigMap/Map_BIGMap.h"

MapData S_Maps[] = 
{
	{
		64,
		512,
		Map_TallMap_Tiles,
		Map_TallMap_TilesLen,
		Map_TallMap_Map,
		Map_TallMap_MapLen,
		true,
		Map_TallMap_Col,
		Map_TallMapColLen,

		&S_Palettes[MAP_TALL_PALETTE_ASSET]
	},
	{
		128,
		128,
		Map_BIGMap_Tiles,
		Map_BIGMap_TilesLen,
		Map_BIGMap_Map,
		Map_BIGMap_MapLen,
		true,
		Map_BIGMap_Col,
		Map_BIGMapColLen,

		&S_Palettes[MAP_BIG_PALETTE_ASSET]
	}
};

typedef struct {u32 data[8]; } MapRow_Half;
void LoadMap(MapData* map, bool loadPal, u8 palBlock, u8 tileBlock, u8 mapBlock)
{
	if(loadPal)
	{
		LoadBackgroundPalette(map->LinkedPalette, palBlock);
	}

	memcpy(getBGTileBlock(tileBlock), map->Tiles, map->TilesLen); // Load the tiles into memory

	MapRow_Half* src = (MapRow_Half*)map->Data;
	MapRow_Half* dst = (MapRow_Half*)getBGMapBlock(mapBlock);

	for( u32 i = 0; i < 32; ++i)
	{
		*dst++ = *src++;   *dst++ = *src++;

		src += (map->Width >> 4) - 2;
	}
	
	REG_BGCNT[0] = setBGControlRegister(0, tileBlock, 0, 0, mapBlock, 0, BG_REG_SIZE_64x64); // Set background control
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

void ScrollMap(MapData* mapData, u16 baseMapBlock, UVec2 camPos)
{
	static UVec2 lastCamPos;

	const u8 ScreenTilesX = 30;
	const u8 ScreenTilesY = 20;

	// TODO: Dont do this
	if(camPos.Y != lastCamPos.Y)
	{
		s8 curOffset = camPos.Y < lastCamPos.Y ? -1 : ScreenTilesY+1;
		u16 mapPosY = ((camPos.Y >> 3) + curOffset) & ((u16)mapData->Height - 1);
		u16 screenPosY = ((camPos.Y >> 3) + curOffset) & (u16)63;		

		for(u8 x = 0; x <= ScreenTilesX; ++x)
		{
			u16 mapPosX = ((camPos.X >> 3) + x) & ((u16)mapData->Width - 1);
			u16 screenPosX = ((camPos.X >> 3) + x) & (u16)63;

			u16* dst = getBGMapBlock(baseMapBlock) + se_index_fast(screenPosX, screenPosY);
			u16 src = mapData->Data[mapPosY * mapData->Width + mapPosX];

			*dst = src;
		}
	}
	if(camPos.X != lastCamPos.X)
	{
		s8 curOffset = camPos.X < lastCamPos.X ? -1 : ScreenTilesX+1;
		u16 mapPosX = ((camPos.X >> 3) + curOffset) & ((u16)mapData->Width - 1);
		u16 screenPosX = ((camPos.X >> 3) + curOffset) & (u16)63;

		// Overdraw for rounding issues
		for(s8 y = -1; y <= ScreenTilesY+1; ++y)
		{
			u16 mapPosY = ((camPos.Y >> 3) + y) & ((u16)mapData->Height - 1);
			u16 screenPosY = ((camPos.Y >> 3) + y) & (u16)63;

			u16* dst = getBGMapBlock(baseMapBlock) + se_index_fast(screenPosX, screenPosY);
			u16 src = mapData->Data[mapPosY * mapData->Width + mapPosX];

			*dst = src;
		}
	}

	lastCamPos = camPos;
}