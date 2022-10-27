#include "input.h"


static const int MultiplyDeBruijnBitPosition2[32] = 
{
  0, 1, 28, 2, 29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4, 8, 
  31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6, 11, 5, 10, 9
};
#define DEF_KEYBIND(actionName) void (*S_Key##actionName[10])(void*);  \
void* S_Key##actionName##Data[10];

DEF_KEYBIND(Held)
DEF_KEYBIND(Down)
DEF_KEYBIND(Up)

#undef DEF_KEYBIND

void BindActionToKeyHeld(u16 actionID, void(*func)(void*), void* extraData)
{
	if(actionID < A || actionID > L)
	{
		return;
	}
	
	int idx = MultiplyDeBruijnBitPosition2[(uint32_t)(actionID * 0x077CB531U) >> 27];
	S_KeyHeld[idx] = func;
	S_KeyHeldData[idx] = extraData;
}

void(*AxisBinds[])(AXIS, s16, void*) = {NULL, NULL};
void(*AxisBindsData[])(void*) = {NULL, NULL};

void BindActionToAxis(AXIS axis, void(*func)(AXIS, s16, void*), void* extraData)
{
	if(axis < 0 || axis > 1)
	{
		return;
	}

	AxisBinds[axis] = func;
	AxisBindsData[axis] = extraData;
}

void UpdateInputBinds()
{
	u16 input = currentKeyState();
	for(int i = 0; i < 10; ++i)
	{
		if(keyDown(1 << i))
		{
			void (*curPressed)(void*) = S_KeyHeld[i];
			if(curPressed != NULL)
			{
				curPressed(S_KeyHeldData[i]);
			}
		}
	}

	for(int i = 0; i < 2; ++i)
	{
		if(AxisBinds[i] != NULL)
		{
			AxisBinds[i](i, getAxis(i), AxisBindsData[i]);
		}
	}
}