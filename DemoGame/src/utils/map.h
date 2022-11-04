#ifndef __MAP_H__
#define __MAP_H__

#include "../gba/gba_types.h"
#include "Palette.h"
#include "vector.h"

// TEMP
#define MAP_TALL_ASSET 0
#define MAP_BIG_ASSET 1

typedef struct MapData
{
	u32 Width;
	u32 Height;
	const u16* Tiles;
	const u32 TilesLen;
	const u16* Data;
	const u32 DataLen;

	bool HasCollisionData;
	const u16* CollisionData;
	const u32 CollisionDataLen;

	PaletteData* LinkedPalette;
}MapData;

extern MapData S_Maps[];

extern void ScrollMap(MapData* mapData, u16 baseMapBlock, UVec2 camPos);
extern void LoadMap(MapData* map, bool loadPal, u8 palBlock, u8 tileBlock, u8 mapBlock);


#endif