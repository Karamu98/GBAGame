#include "texture.h"

#include "../res/CharacterSheet.h"


const PaletteData S_Palettes[] = 
{
	{ CharacterSheetPal, CharacterSheetPalLen }
};

const Texture S_Textures[] = 
{
	{
		CharacterSheetTiles,
		CharacterSheetTilesLen,
		true,
		32, 32,
		3,
		192,
		&S_Palettes[0]
	}
};

const Sequence S_Sequences[] =
{
	{
		CharacterSheetSequence,
		CharacterSheetSequenceLen
	}
};