#include "vector.h"

Vec2 AddVec2(const Vec2* first, const Vec2* second) { return (Vec2){ first->X + second->X, first->Y + second->Y}; }

UVec2 AddUVec2(const UVec2* first, const UVec2* second) { return (UVec2){ first->X + second->X, first->Y + second->Y}; }
