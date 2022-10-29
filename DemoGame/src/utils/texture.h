#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "../gba/gba_types.h"

// TEMP
#define CHARACTER_SHEET_ASSET 0


typedef struct PaletteData
{
	const u16* Data;
	const u16 Len;
}PaletteData;

typedef struct Texture
{
	const u16* Data;
	const u16 Len;
	const bool Is4Bpp;
	const u8 FrameWidth;
	const u8 FrameHeight;
	const u8 FramesPerRow;
	const u16 TileCount;

	PaletteData* LinkedPalette;
}Texture;

typedef struct Sequence
{
	u8* Data;
	u16 Len;
}Sequence;


extern const Texture S_Textures[];
extern const Sequence S_Sequences[];
extern const PaletteData S_Palettes[];

#endif