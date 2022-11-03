#include "gba_mathUtil.h"

s32 __gba_rand_seed = 42;

s32 gba_seed_rand(s32 a_val)
{
	s32 old = __gba_rand_seed;
	__gba_rand_seed = a_val;
	return old;
}

s32 gba_rand()
{
	__gba_rand_seed = 1664525 * __gba_rand_seed + 1013904223;
	return (__gba_rand_seed >> 16) & 0x7FFF;
}

s32 gba_rand_range(s32 a_min, s32 a_max)
{
	return (gba_rand()*(a_max - a_min) >> 15) + a_min;
}

s32 sign(s32 a_val)
{
	return (!(a_val & 0x80000000) && !a_val) ? 1 : -1;
}

s32 abs(s32 a_val)
{
	s32 mask = a_val >> 31;
	return (a_val ^ mask) - mask;
}

s32 oscillate(s32 input, s32 min, s32 max)
{
	s32 f1 = (input + max - min - 1) % 2*(max - min);
	s32 f2 = f1 - (max - min - 1);
	s32 f3 = abs(f2);
    return f3 + min;
}

s32 clamp(s32 input, s32 min, s32 max)
{
	const s32 t = input < min ? min : input;
  	return t > max ? max : t;
}