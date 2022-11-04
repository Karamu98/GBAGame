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


#include "gba/gba_gfx.h"

typedef struct GameData
{
	PlayerEntityData Player;
	EntityData NPCData;
	Camera Camera;
}GameData;

static GameData S_GameData;

static u8 S_SELECTEDMAP = MAP_BIG_ASSET;

void InitGameData()
{
	LoadSpritePalette(&S_Palettes[CHARACTER_SHEET_PALETTE_ASSET], 0);
	LoadMap(&S_Maps[S_SELECTEDMAP], true, 0, 0, 16);

	InitPlayer(&S_GameData.Player, 0, 0);

	S_GameData.Camera.FollowTarget = &S_GameData.Player.Entity.Sprite.Transform;
	u8 newX = (SCREEN_W >> 1) - (S_Textures[CHARACTER_SHEET_ASSET].FrameWidth >> 1);
	u8 newY = (SCREEN_H >> 1) - (S_Textures[CHARACTER_SHEET_ASSET].FrameHeight >> 1);
	S_GameData.Player.Entity.Sprite.Transform.Position = (Vec2){newX, newY};
	S_GameData.Camera.FollowOffset.Position = (Vec2){newX, newY};
}

void GameLoop()
{
	UpdatePlayer(&S_GameData.Player);
}

void LateUpdate()
{
	if(keyHit(A))
	{
		UpdateCamera(&S_GameData.Camera, S_SELECTEDMAP, 16);
		UVec2 curCamPos = *(UVec2*)&S_GameData.Camera.Transform.Position;
		RefreshMap(&S_Maps[S_SELECTEDMAP], 16, curCamPos);
	}
	if(keyDown(A))
	{
		UpdateCamera(&S_GameData.Camera, S_SELECTEDMAP, 16);
	}

	DrawSprite(&S_GameData.Player.Entity.Sprite, &S_GameData.Camera);
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

		oam_copy(MEM_OAM, obj_buffer, 1);
	}
	return 0;
}
