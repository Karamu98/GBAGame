#include "camera.h"

void UpdateCamera(Camera* camera)
{
	if(camera->FollowTarget != 0)
	{
		camera->Transform.Position = AddVec2(&camera->FollowTarget->Position, &camera->FollowOffset.Position);
	}
}