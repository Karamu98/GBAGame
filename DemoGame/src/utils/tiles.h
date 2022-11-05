#ifndef __TILES_H__
#define __TILES_H__

#include "../gba/gba_types.h"

#define CHARACTER_SHEET_TILES_ASSET 0
#define KNIGHT_TILES_ASSET 1
#define MAGE_TILE_ASSET 2

typedef struct Tiles
{
	const u16* Data;
	const u16 Len;
	const bool Is4Bpp;
	const u16 TileCount;
}Tiles;

extern Tiles S_Tiles[];

extern u16 LoadTiles(Tiles* target, u16* memTarget, u16* outOffset);
extern u16 LoadSpriteTiles(Tiles* target, u16 tileIDX, u16* outOffset);
extern u16 LoadBackgroundTiles(Tiles* target, u16 tileIDX, u16* outOffset);

#endif