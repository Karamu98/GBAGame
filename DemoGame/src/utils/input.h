#ifndef __UTIL_INPUT_H__
#define __UTIL_INPUT_H__

#include "../gba/gba_input.h"

void UpdateInputBinds();
void BindActionToKeyHeld(u16 actionID, void(*func)(void*), void* extraData);
void BindActionToAxis(AXIS axis, void(*func)(AXIS, s16, void*), void* extraData);

#endif