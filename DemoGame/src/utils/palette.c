#include "palette.h"

#include <string.h>

#include "../gba/gba_gfx.h"

#include "../res/CharacterSheet.h"
#include "../res/maps/TallMap/Map_TallMap.h"
#include "../res/maps/BigMap/Map_BIGMap.h"

PaletteData S_Palettes[] = 
{
	{ CharacterSheetPal, CharacterSheetPalLen },
	{ Map_TallMap_Palette, Map_TallMap_PaletteLen},
	{ Map_BIGMap_Palette, Map_BIGMap_PaletteLen}
};

void pal_init(PaletteData* self, u16* memSlot) { memcpy(memSlot, self->Data, self->Len); }

extern void LoadSpritePalette(PaletteData* self, u8 slot)
{
	pal_init(self, PAL_SP_BLOCK(slot));
}

extern void LoadBackgroundPalette(PaletteData* self, u8 slot)
{
	pal_init(self, PAL_BG_BLOCK(slot));
}