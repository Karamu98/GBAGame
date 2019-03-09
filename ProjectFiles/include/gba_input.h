//---------------------------------------------------------------------------------
// This header file outlines the registers for GBA Key Input and provides functionality
// for handling key presses
//
//---------------------------------------------------------------------------------

#include "gba_reg.h"
#include "gba_types.h"

#ifndef __GBA_INPUT_H__
#define __GBA_INPUT_H__
/*
* Define for Hardware Key Input Register
*/
#define REG_KEYINPUT	*(vu16*)(REG_BASE + 0x130)
/*
* Define for Hardware Key Interrupt Control Register
*/
#define REG_KEYCNT		*(vu16*)(REG_BASE + 0x132)

typedef enum KEYS
{
	A = (1 << 0),
	B = (1 << 1),
	SELECT = (1 << 2),
	START = (1 << 3),
	RIGHT = (1 << 4),
	LEFT = (1 << 5),
	UP = (1 << 6),
	DOWN = (1 << 7),
	R = (1 << 8),
	L = (1 << 9),

	KEYIRQ_ENABLE = (1 << 14),	/*!< Enable keypad interrupt */
	KEYIRQ_OR = (0 << 15),		/*!< interrupt logical OR mode */
	KEYIRQ_AND = (1 << 15),		/*!< interrupt logical AND mode */


}KEYS;

#define KEY_MASK 0x03FF
//Vars to hold the current and previous state of the hardware keys
extern u16 __currKeys, __prevKeys;
//Function to test for Hardware Key Depress
INLINE void PollKeys()
{
	__prevKeys = __currKeys;
	__currKeys = ~REG_KEYINPUT & KEY_MASK;
}


INLINE u16	currentKeyState()
{
	return __currKeys;
}

INLINE u16	prevKeyState()
{
	return __prevKeys;
}

INLINE u32	keyDown(u32 a_key)
{
	return __currKeys & a_key;
}

INLINE u32	keyUp(u32 a_key)
{
	return ~__currKeys & a_key;
}

INLINE u32	keyHeld(u32 a_key)
{
	return (__currKeys & __prevKeys) & a_key;
}

INLINE u32	keyReleased(u32 a_key)
{
	return (~__currKeys & __prevKeys) & a_key;
}

INLINE u32	keyHit(u32 a_key)
{
	return (__currKeys & ~__prevKeys) & a_key;
}

INLINE u32	keyStateChange(u32 a_key)
{
	return (__currKeys ^ __prevKeys) & a_key;
}

typedef enum AXIS
{
	HORIZONTAL = 0,
	VERTICAL,

}AXIS;

extern s32	getAxis(AXIS a_val);
#endif //__GBA_INPUT_H__