#ifndef __MAP_H__
#define __MAP_H__

#include "../gba/gba_types.h"
#include "Palette.h"

// TEMP
#define MAP_TALL_ASSET 0
#define MAP_BIG_ASSET 1

typedef struct MapData
{
	u32 Width;
	u32 Height;
	u16* Tiles;
	u32 TilesLen;
	u16* Data;
	u32 DataLen;

	bool HasCollisionData;
	u16* CollisionData;
	u32 CollisionDataLen;

	PaletteData* LinkedPalette;
}MapData;

extern const MapData S_Maps[];


#endif