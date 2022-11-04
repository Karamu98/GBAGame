#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "../gba/gba_types.h"
#include "transform.h"

typedef struct Camera
{
	Transform Transform;
	Transform* FollowTarget;
	Transform FollowOffset;
}Camera;

extern void UpdateCamera(Camera* camera);

#endif