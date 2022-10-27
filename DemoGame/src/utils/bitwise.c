#include "bitwise.h"

u16 BitSetU16ByMask(u16 curVal, u16 newVal, u16 mask)
{
	return (curVal & ~mask) | (newVal & mask);
}

u16 BitGetU16ByMask(u16 val, u16 mask)
{
	return val & mask;
}