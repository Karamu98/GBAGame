#include "camera.h"
#include "map.h"
#include "../gba/gba_gfx.h"

void UpdateCamera(Camera* camera, u8 selectedMap, u16 baseMapBlock)
{
	if(camera->FollowTarget != 0)
	{
		camera->Transform.Position = SubVec2(&camera->FollowTarget->Position, &camera->FollowOffset.Position);
	}

	UVec2 curCamPos = *(UVec2*)&camera->Transform.Position;
	ScrollMap(&S_Maps[selectedMap], baseMapBlock, curCamPos);

	REG_BG_OFFSET[0].x = curCamPos.X;
	REG_BG_OFFSET[0].y = curCamPos.Y;
}