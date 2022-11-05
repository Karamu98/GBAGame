#include "tiles.h"

#include <string.h>

#include "../gba/gba_gfx.h"

#include "../res/CharacterSheet.h"
#include "../res/Knight.h"
#include "../res/Mage.h"

Tiles S_Tiles[] = 
{
	{
		CharacterSheetTiles,
		CharacterSheetTilesLen,
		true,
		192
	},
	{
		KnightTiles,
		KnightTilesLen,
		true,
		192
	},
	{
		MageTiles,
		MageTilesLen,
		true,
		192
	}
};

u16 LoadTiles(Tiles* target, u16* memTarget, u16* outOffset)
{
	if(outOffset != 0)
	{
		*outOffset += target->TileCount;
	}
	memcpy(memTarget, target->Data, target->Len);
	return target->TileCount;
}

u16 LoadSpriteTiles(Tiles* target, u16 tileIDX, u16* outOffset)
{
	return LoadTiles(target, sprite_tile_address(tileIDX), outOffset);
}

u16 LoadBackgroundTiles(Tiles* target, u16 tileIDX, u16* outOffset)
{
	return LoadTiles(target, getBGTileBlock(tileIDX), outOffset);
}