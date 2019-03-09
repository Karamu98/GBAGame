#include "gba_input.h"

//set the extern valuse for key values
u16 __currKeys = 0, __prevKeys = 0;

s32		getAxis(AXIS a_val)
{
	switch (a_val)
	{
	case HORIZONTAL:
		//shift __currKeys down 4 to move the value for RIGHT to the lowest bit & with 1 do the same for LEFT and subtract
		//if right is pressed the equation becomes 1 - 0 = 1, if Left is pressed then 0 - 1 = -1.
		return ((__currKeys >> 4) & 1) - ((__currKeys >> 5) & 1);
	case VERTICAL:
		//This is the same logic as above however uses UP and DOWN.
		return ((__currKeys >> 6) & 1) - ((__currKeys >> 7) & 1);
	default:
		return 0;
	}
}