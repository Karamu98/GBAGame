#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "../gba/gba_types.h"
#include "palette.h"
#include "tiles.h"

// TEMP
#define CHARACTER_SHEET_TEXTURE_ASSET 0
#define KNIGHT_TEXTURE_ASSET 1
#define MAGE_TEXTURE_ASSET 2

#define CHARACTER_SHEET_SEQUENCE_ASSET 0

typedef struct Texture
{
	const Tiles* Tiles;
	const u8 FrameWidth;
	const u8 FrameHeight;
	const u8 FramesPerRow;

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