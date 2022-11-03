#include "map.h"

#include "../res/maps/TallMap/Map_TallMap.h"
#include "../res/maps/BigMap/Map_BIGMap.h"

const MapData S_Maps[] = 
{
	{
		64,
		512,
		Map_TallMap_Tiles,
		Map_TallMap_TilesLen,
		Map_TallMap_Map,
		Map_TallMap_MapLen,
		true,
		Map_TallMap_Col,
		Map_TallMapColLen,

		&S_Palettes[MAP_TALL_PALETTE_ASSET]
	},
	{
		128,
		128,
		Map_BIGMap_Tiles,
		Map_BIGMap_TilesLen,
		Map_BIGMap_Map,
		Map_BIGMap_MapLen,
		true,
		Map_BIGMap_Col,
		Map_BIGMapColLen,

		&S_Palettes[MAP_BIG_ASSET]
	}
};