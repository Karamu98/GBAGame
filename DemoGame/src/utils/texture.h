#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "../gba/gba_types.h"
#include "palette.h"

// TEMP
#define CHARACTER_SHEET_ASSET 0

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
	const u8* Data;
	const u16 Len;
}Sequence;


extern Texture S_Textures[];
extern Sequence S_Sequences[];

#endif