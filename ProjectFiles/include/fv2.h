#ifndef _FV2_H_
#define _FV2_H_

#include "gba_math.h"

typedef struct fv2
{
	fixed fX;
	fixed fY;
}fv2;

extern fv2 fv2Create(fixed a_x, fixed a_y); // Creates a fv2
//extern u8 DistanceBetween(fv2 a_a, fv2 a_b); // Gets distance between two points


	extern fv2 fvAdd(fv2 fv, fv2 fv1); // Adds two fv2 values together
	extern fv2 fvSub(fv2 fv, fv2 fv1); // Subtracts two fv2 values from eachother
	extern fv2 fvMul(fv2 fv, fv2 fv1); // Multiplies a fv2 value by the other
	extern fv2 fvDiv(fv2 fv, fv2 fv1); // Divides an fv2 values by the other

	extern fv2 fvInvert(fv2 fv);
#endif