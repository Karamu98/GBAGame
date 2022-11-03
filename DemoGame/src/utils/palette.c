#include "palette.h"

#include "../res/CharacterSheet.h"
#include "../res/maps/TallMap/Map_TallMap.h"
#include "../res/maps/BigMap/Map_BIGMap.h"

const PaletteData S_Palettes[] = 
{
	{ CharacterSheetPal, CharacterSheetPalLen },
	{ Map_TallMap_Palette, Map_TallMap_PaletteLen},
	{ Map_BIGMap_Palette, Map_BIGMap_PaletteLen}
};