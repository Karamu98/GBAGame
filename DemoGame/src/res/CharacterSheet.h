
//{{BLOCK(CharacterSheet)

//======================================================================
//
//	CharacterSheet, 96x128@4, 
//	+ palette 16 entries, not compressed
//	+ 192 tiles Metatiled by 4x4 not compressed
//	Total size: 32 + 6144 = 6176
//
//	Time-stamp: 2017-11-23, 16:38:02
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.3
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_CHARACTERSHEET_H
#define GRIT_CHARACTERSHEET_H

#define CharacterSheetTilesLen 6144
extern const unsigned short CharacterSheetTiles[3072];

#define CharacterSheetPalLen 32
extern const unsigned short CharacterSheetPal[16];

#define CharacterSheetSequenceLen 4
const uint8_t CharacterSheetSequence[] = {1, 2, 1, 0};

#define CharacterFramesPerRow 3

#endif // GRIT_CHARACTERSHEET_H

//}}BLOCK(CharacterSheet)
