#ifndef _MEMORYMANAGER_H_
#define _MEMORYMANAGER_H_

#include "gba_types.h"

// Using bool = 1152 bytes
// Using bitfield = 144 bytes (87.5% less space)


u8 TileArray[128]; // 8 bool values per index (128 bytes compared to 1kb)
bool bTileArray[1024]; // Because u8 want able to get fixed in time :(
u8 SpriteArray[16]; // 8 vool values per index (16 bytes compared to 128 bytes)



extern void InitManager(); // Initiate all values false

extern u16 GetFreeTile(); // Finds the first free tile in memory and returns ID
extern u16 GetFreeTileWSpace(u16 a_requiredTiles); // Finds a space with x avalible tiles to use
extern void TagTile(u16 a_startTile, u16 a_endTile); // Mark that these tiles are being used for GetFreeTile's next query
extern void DropTile(u16 a_startTile, u16 a_endTile); // Mark that these tiles are now free again to write over

extern u8 GetFreeSprite(); // Finds the first free sprite in memory and returns ID
extern void TagSprite(u8 a_spriteID); // Mark that this sprite is being used for GetFreeSprite's next query
extern void DropSprite(u8 a_spriteID); // Mark that this sprite is now free again to write over




#endif