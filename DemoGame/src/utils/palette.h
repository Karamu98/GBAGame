#ifndef __PALETTE_H__
#define __PALETTE_H__

#include "../gba/gba_types.h"

// TEMP
#define CHARACTER_SHEET_PALETTE_ASSET 0
#define MAP_TALL_PALETTE_ASSET 1
#define MAP_BIG_PALETTE_ASSET 2

typedef struct PaletteData
{
	const u16* Data;
	const u16 Len;
}PaletteData;

extern PaletteData S_Palettes[];

extern void LoadSpritePalette(PaletteData* self, u8 slot);
extern void LoadBackgroundPalette(PaletteData* self, u8 slot);

#endif