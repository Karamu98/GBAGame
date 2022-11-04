#include "collision.h"

bool IsRectOverlapped(const Rect* self, const Rect* other)
{
	return self->X < other->X + other->W &&
    self->X + self->W > other->X &&
    self->Y < other->Y + other->H &&
    self->H + self->Y > other->Y;
}