#include "texture.h"

#include "../res/CharacterSheet.h"

Texture S_Textures[] = 
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

Sequence S_Sequences[] =
{
	{
		CharacterSheetSequence,
		CharacterSheetSequenceLen
	}
};