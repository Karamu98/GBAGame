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

	u8 newX = (SCREEN_W >> 1) - (S_Textures[CHARACTER_SHEET_ASSET].FrameWidth >> 1);
	u8 newY = (SCREEN_H >> 1) - (S_Textures[CHARACTER_SHEET_ASSET].FrameHeight >> 1);
	SetSpritePosition(S_GameData.Player.Entity.Sprite.ID, newX, newY);
}

void GameLoop()
{
	UpdatePlayer(&S_GameData.Player);

	S_GameData.Camera.Transform.Position.X += getAxis(HORIZONTAL) << 1;
	S_GameData.Camera.Transform.Position.Y += -getAxis(VERTICAL) << 1;

	UVec2 curCamPos = *(UVec2*)&S_GameData.Camera.Transform.Position;
	ScrollMap(&S_Maps[S_SELECTEDMAP], 16, curCamPos);

	REG_BG_OFFSET[0].x = curCamPos.X;
	REG_BG_OFFSET[0].y = curCamPos.Y;
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

		oam_copy(MEM_OAM, obj_buffer, 1);
	}
	return 0;
}
