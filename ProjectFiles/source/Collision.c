#include "Collision.h"

bool AABBDynCollision(DynSprite* a_sprite1, DynSprite* a_sprite2) // Tests two moving sprites to test for their collision
{
	if (a_sprite1->vPosition.fX < a_sprite2->vPosition.fX + a_sprite2->vSize.fX &&
		a_sprite1->vPosition.fX + a_sprite1->vSize.fX > a_sprite2->vPosition.fX &&
		a_sprite1->vPosition.fY < a_sprite2->vPosition.fY + a_sprite2->vSize.fY &&
		a_sprite1->vSize.fY + a_sprite1->vPosition.fY > a_sprite2->vPosition.fY
		)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool AABBStaticCollision(DynSprite* a_sprite, Sprite* a_staticsprite) // Tests a moving sprite against a static sprite testing for collision
{
	if (a_sprite->vPosition.fX < a_staticsprite->vPosition.fX + a_staticsprite->vSize.fX &&
		a_sprite->vPosition.fX + a_sprite->vSize.fX > a_staticsprite->vPosition.fX &&
		a_sprite->vPosition.fY < a_staticsprite->vPosition.fY + a_staticsprite->vSize.fY &&
		a_sprite->vSize.fY + a_sprite->vPosition.fY > a_staticsprite->vPosition.fY
		)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool DynCircleCollision(DynSprite* a_circleSprite, DynSprite* a_sprite) // Tests a circular moving sprite against a moving sprite
{
	/*if (DistanceBetween(a_circleSprite->vPosition, a_sprite->vPosition) < a_circleSprite->vSize.fX)
	{
		return true;
	}
	else
	{
		return false;
	}*/
	return false;
}

bool StaticCircleCollision(DynSprite* a_circleSprite, Sprite* a_sprite) // Tests a circular moving sprite against a static sprite
{
	/*if (DistanceBetween(a_circleSprite->vPosition, a_sprite->vPosition) < a_circleSprite->vSize.fX)
	{
		return true;
	}
	else
	{
		return false;
	}*/
	return false;
}
