#include "entity.h"


void InitEntity(EntityData* self, u8 x, u8 y, Texture* texRef, u16 tileIDX, u16 palIDX)
{
	InitSprite(&self->Sprite, GetFreeSprite(), x, y, texRef, tileIDX, palIDX);
}