#include "entity.h"


void InitEntity(EntityData* self, u16 x, u16 y, u16 w, u16 h, u16 colour)
{
	InitSprite(&self->Sprite, GetFreeSprite(), fv2Create(x, y), fv2Create(w, h));
	self->Colour = colour;
}