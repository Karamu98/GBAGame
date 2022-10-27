#include "gba/gba.h"
#include <string.h>
#include "gba/gba_math.h"
#include "utils/input.h"
#include "utils/sprite_manager.h"
#include "utils/animation.h"
#include "player.h"


typedef struct GameData
{
	PlayerEntityData Player;
	EntityData CpuData;
}GameData;

static GameData S_GameData;


void InitGameData()
{
	InitPlayer(&S_GameData.Player);
	InitEntity(&S_GameData.CpuData, 10, SCREEN_W - 60, 8, 40, setColor(31, 0, 0));
}

void GameLoop()
{
	UpdatePlayer(&S_GameData.Player);
}

int main()
{
	REG_DISPCNT = VIDEOMODE_0 | ENABLE_OBJECTS | MAPPING_MODE_1D;

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
