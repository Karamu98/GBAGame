#include "gba/gba.h"
#include <string.h>
#include "gba/gba_math.h"
#include "gba/gba_mathUtil.h"
#include "utils/input.h"
#include "utils/sprite_manager.h"
#include "utils/animation.h"
#include "player.h"
#include "utils/map.h"
#include "utils/camera.h"
#include <string.h>


#include "gba/gba_gfx.h"

#define NPC_COUNT 5

typedef struct GameData
{
	EntityData Player;
	AnimationUniqueData PlayerAnim;

	EntityData NPCData[NPC_COUNT];
	Camera Camera;
}GameData;

static GameData S_GameData;

static u8 S_SELECTEDMAP = MAP_BIG_ASSET;

void InitGameData()
{
/// ===========================================================
/// This block should eventually be done through loading a 
/// scene file `LoadScene(S_SCENES[SCENE_LEVEL_1])` etc 

	LoadSpritePalette(&S_Palettes[CHARACTER_SHEET_PALETTE_ASSET], 0);
	LoadSpritePalette(&S_Palettes[KNIGHT_PALETTE_ASSET], 1);

	u16 offset = 16;
	u16 knightTiles = offset;
	LoadSpriteTiles(&S_Tiles[KNIGHT_TILES_ASSET], offset, &offset);
	u16 mageTiles = offset;
	LoadSpriteTiles(&S_Tiles[MAGE_TILE_ASSET], offset, 0);

	LoadMap(&S_Maps[S_SELECTEDMAP], true, 0, 0, 16);

	u8 newX = (SCREEN_W >> 1) - (S_Textures[CHARACTER_SHEET_TEXTURE_ASSET].FrameWidth >> 1);
	u8 newY = (SCREEN_H >> 1) - (S_Textures[CHARACTER_SHEET_TEXTURE_ASSET].FrameHeight >> 1);

	InitSprite(&S_GameData.Player.Sprite, (Vec2){newX, newY}, GetFreeSprite(), &S_Textures[CHARACTER_SHEET_TEXTURE_ASSET], 0, 0, false, false);
	InitUniqueAnimation(&S_GameData.PlayerAnim, sprite_tile_address(0), &S_Textures[CHARACTER_SHEET_TEXTURE_ASSET], &S_Sequences[CHARACTER_SHEET_SEQUENCE_ASSET], 7);
	InitPlayer(&S_GameData.Player);

	bool flip = true;
	for(u32 i = 0; i < NPC_COUNT; i++)
	{
		InitSprite(&S_GameData.NPCData[i].Sprite, (Vec2){(30<<3)+ i*(4<<3), 18<<3}, GetFreeSprite(), &S_Textures[flip ? KNIGHT_TEXTURE_ASSET : MAGE_TEXTURE_ASSET], flip ? knightTiles : mageTiles, 1, false, false);
		flip = !flip;
	}

	// Set up camera follow/offset
	S_GameData.Camera.FollowTarget = &S_GameData.Player.Sprite.Transform;
	S_GameData.Camera.FollowOffset.Position = (Vec2){newX, newY};

/// ===========================================================
}

void GameLoop()
{
	UpdatePlayer(&S_GameData.Player, &S_GameData.PlayerAnim);
}

void LateUpdate()
{
	if(keyReleased(A))
	{
		UpdateCamera(&S_GameData.Camera, S_SELECTEDMAP, 16);
		UVec2 curCamPos = *(UVec2*)&S_GameData.Camera.Transform.Position;
		RefreshMap(&S_Maps[S_SELECTEDMAP], 16, curCamPos);
	}
	if(!keyDown(A))
	{
		UpdateCamera(&S_GameData.Camera, S_SELECTEDMAP, 16);
	}

	DrawUniqueAnimation(&S_GameData.PlayerAnim);
	DrawSprite(&S_GameData.Player.Sprite, &S_GameData.Camera);
	for(u32 i = 0; i < NPC_COUNT; i++)
	{
		DrawSprite(&S_GameData.NPCData[i].Sprite, &S_GameData.Camera);
	}	
}

int main()
{
	REG_DISPCNT = VIDEOMODE_0 | ENABLE_OBJECTS | MAPPING_MODE_1D | BGMODE_0;

	register_vblank_isr();
	oam_init(obj_buffer, 128);
	InitGameData();

	while (1)
	{
		vblank_intr_wait();
		
		PollKeys();
		UpdateInputBinds();

		GameLoop();

		LateUpdate();

		oam_copy(MEM_OAM, obj_buffer, 128);
	}
	return 0;
}
