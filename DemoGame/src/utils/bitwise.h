#ifndef __BITWISE_H__
#define __BITWISE_H__

#include "../gba/gba_types.h"

extern u16 BitSetU16ByMask(u16 curVal, u16 newVal, u16 mask);
extern u16 BitGetU16ByMask(u16 val, u16 mask);

#endif