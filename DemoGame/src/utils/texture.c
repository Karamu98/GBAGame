#include "texture.h"

#include "../res/CharacterSheet.h"
#include "../res/Knight.h"
#include "../res/Mage.h"

Texture S_Textures[] = 
{
	{
		&S_Tiles[CHARACTER_SHEET_TILES_ASSET],
		32, 32,
		3,
		&S_Palettes[CHARACTER_SHEET_PALETTE_ASSET]
	},
	{
		&S_Tiles[KNIGHT_TILES_ASSET],
		32, 32,
		3,
		&S_Palettes[KNIGHT_PALETTE_ASSET]
	},
	{
		&S_Tiles[MAGE_TILE_ASSET],
		32, 32,
		3,
		&S_Palettes[MAGE_PALETTE_ASSET]
	}
};

Sequence S_Sequences[] =
{
	{
		CharacterSheetSequence,
		CharacterSheetSequenceLen
	}
};