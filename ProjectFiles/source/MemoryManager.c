#include "MemoryManager.h"


void InitManager()
{
	for (int i = 0; i < 1024; i++)
	{
		bTileArray[i] = false;
	}

	for (int i = 0; i < 128; i++)
	{
		TileArray[i] = 0; // All bits off
	}

	for (int i = 0; i < 16; i++)
	{
		SpriteArray[i] = 0; // All bits off
	}
}

u16 GetFreeTile()
{
	for (u16 i = 0; i < 1024; i++)
	{
		if (bTileArray[i] == false)
		{
			return i;
		}
	}
}

u16 GetFreeTileWSpace(u16 a_requiredTiles)
{
	u16 counter = 0;
	for (u16 i = 0; i < 1024; i++)
	{
		if (bTileArray[i] == false)
		{
			counter++;
		}
		if (bTileArray[i] == true)
		{
			counter = 0;
		}

		if (counter == a_requiredTiles)
		{
			return i - (counter - 1);
		}
	}
}

void TagTile(u16 a_startTile, u16 a_endTile)
{
	for (int i = a_startTile; i < a_endTile; i++)
	{
		bTileArray[i] = true;
	}
}

void DropTile(u16 a_startTile, u16 a_endTile)
{
	for (int i = a_startTile; i < a_endTile; i++)
	{
		bTileArray[i] = false;
	}
}

/*u16 GetFreeTile()
{
	u8 currentValue;
	u8 mask;
	for (int i = 0; i < 128; i++)
	{
		u8 currentValue = TileArray[i];
		for (int j = 0; j < 8; j++)
		{
			mask = 1 << j;
			if (currentValue & mask != 1)
			{
				return (i >> 3) + j;
			}
		}
	}
}*/


/*u16 GetFreeTileWSpace(u16 a_requiredTiles)
{	
	u8 mask = 0;

	u16 counter = 0;
	for (u8 i = 0; i < 128; ++i)
	{
		for (u8 j = 0; j < 8; ++j)
		{
			mask = TileArray[i] | (1 << j); // Create the mask to test with 00000001 (0) where 1 is moved by j 00000010 (1)

			if ((TileArray[i] | mask) != TileArray[i]) // 00100000 & 00000001 = 00000000		00000100 & 00000100 = 00000100 (4)
			{
				counter++;
			}
			else if((TileArray[i] | mask) == TileArray[i])
			{
				counter = 0;
			}

			if (counter == a_requiredTiles)
			{
				counter--;
				return (((i << 3) + j) - counter);
			}
		}
	}

	// If we havent found any space
	//return 129; // Will Test this on tag
}
*/
/*void TagTile(u16 a_startTile, u16 a_endTile)
{
	u8 arrayStart = a_startTile >> 3; // Get what array value this tile belongs
	u8 arrayEnd = a_endTile >> 3; // Get what array value this tile belongs

	u8 tileAdd = (arrayStart << 3); // Get first tile of that array
	u8 tileEnd = (arrayEnd << 3); // Get last tile of that array


	if ((arrayEnd - arrayStart) > 0) // If requires more than one u8 
	{

		for (int i = arrayStart; i < arrayEnd; i++) // Effects what array member should be accessed
		{
			if (i == arrayStart)
			{
				u8 startCounter = a_startTile - tileAdd;

				for (int j = startCounter; j < 8; j++)
				{
					u8 mask = 1 << j;
					TileArray[i] = (TileArray[i] | mask);
				}
			} 
			else if (i == arrayEnd)
			{
				u8 endCounter = a_endTile - tileEnd;

				for (int j = 0; j < endCounter; j++)
				{
					u8 mask = 1 << j;
					TileArray[i] = (TileArray[i] | mask);
				}
			}
			else if(i != arrayStart || i != arrayEnd)
			{
				TileArray[i] = 255; // Fill
			}
		}

	}
	else if ((arrayEnd - arrayStart) == 0) // If requires only one u8
	{
			u8 startCounter = a_startTile - tileAdd;
			u8 endCount = a_endTile - tileEnd;

			for (int j = startCounter; j < endCount; j++)
			{
				u8 mask = 1 << j;
				TileArray[arrayStart] = (TileArray[arrayStart] | mask);
			}
	}
}
*/
/*void DropTile(u16 a_startTile, u16 a_endTile)
{
	u8 arrayStart = a_startTile >> 3; // Get what array value this tile belongs
	u8 arrayEnd = a_endTile >> 3; // Get what array value this tile belongs

	u8 tileAdd = (arrayStart << 3); // Get first tile of that array
	u8 tileEnd = (arrayEnd << 3); // Get last tile of that array


	if ((arrayEnd - arrayStart) != 0) // If requires more than one u8 
	{

		for (int i = arrayStart; i < arrayEnd; i++) // Effects what array member should be accessed
		{
			if (i == arrayStart) // BUGTWO
			{
				u8 startCounter = a_startTile - tileAdd;

				for (int j = startCounter; j < 8; j++)
				{
					u8 mask = 1 << j;

					TileArray[i] = TileArray[i] & (~mask);
				}
			}
			else if (i == arrayEnd) // BUGTWO
			{
				u8 endCounter = (a_endTile - tileEnd);

				for (int j = 0; j < endCounter; j++)
				{
					u8 mask = 1 << j;
					TileArray[i] = TileArray[i] & (~mask);
				}
			}
			else if (i != arrayStart || i != arrayEnd)
			{
				TileArray[i] = 0; // Empty
			}
		}
		
	}
	else if ((arrayEnd - arrayStart) == 0) // If requires only one u8 BUGTWO
	{
		u8 startCounter = (a_startTile + tileAdd);
		u8 endCount = (a_endTile - tileEnd);

		for (int j = startCounter; j < endCount; j++)
		{
			u8 mask = 1 << j;
			TileArray[arrayStart] = TileArray[arrayStart] & (~mask);
		}
	}
}
*/
u8 GetFreeSprite()
{
	for (u8 i = 0; i < 16; i++) // Get element in array
	{
		for (u8 j = 0; j < 8; j++) // Get bit in u8
		{
			u8 mask = (SpriteArray[i] & (1 << j)); // Create the mask to test with 00000001 (0) where 1 is moved by j 00000010 (1)

			if (mask == 0) 
			{
				return (i << 3) + j;
			}
		}
	}
}

void TagSprite(u8 a_spriteID) // 59
{
	u8 arrayPos = a_spriteID >> 3; // Get array value

	u8 bitValue = 0;
	for (u8 i = (arrayPos << 3); i < a_spriteID; i++) // Get bit position, Round down to closest 8th count up to current tile (TILE 59, count from 56 to 59)
	{
		bitValue++; // Counts
	}

	u8 mask = (1 << bitValue); // Create the mask to test with 00000001 (0) where 1 is moved by j 00000010 (1)

	SpriteArray[arrayPos] = (SpriteArray[arrayPos] | mask); // Apply to u8 value
}

void DropSprite(u8 a_spriteID)
{
	u8 bitValue; // Stores what bit to edit
	u8 arrayPos = (a_spriteID >> 3);

	for (u8 i = arrayPos << 3; i < a_spriteID; i++) // Get bit position
	{
		bitValue++; // Counts
	}

	u8 mask = 1 << bitValue; // Make a mask based on bitValue

	SpriteArray[arrayPos] = SpriteArray[arrayPos] & (~mask); // Apply to u8 value
}
