#include "gba_math.h"
#include "sinlut.h"

//convert and integer to a fixed point representation
fixed	int2fix(s32 a_i)
{
	return a_i << FIX_SHIFT;
}
//Convert a float to a fixed point representation
fixed	float2fix(float a_f)
{
	return (fixed)(a_f*FIX_SCALEF);
}

//convert a fixed point value into an integer value
u32		fix2uint(fixed a_f)
{
	return a_f >> FIX_SHIFT;
}
//Get only the fractional component of a fixed point value as an integer
u32		fix2ufrac(fixed a_f)
{
	return a_f & FIX_MASK;
}
//convert a fixed point number into a signed int
s32		fix2int(fixed a_f)
{
	return a_f / FIX_SCALE;
}

float	fix2float(fixed a_f)
{
	return a_f / FIX_SCALEF;
}

fixed	fixSqrt(fixed number)
{
	fixed start = 0, end = number;
    fixed mid;
 
    // To store the answer
    fixed ans;
 
    // To find integral part of square
    // root of number
    while (start <= end) {
 
        // Find mid
        mid = (start + end) / 2;
 
        // If number is perfect square
        // then break
        if (mid * mid == number) {
            ans = mid;
            break;
        }
 
        // Increment start if integral
        // part lies on right side
        // of the mid
        if (mid * mid < number) {
          //first start value should be added to answer
            ans=start;
          //then start should be changed
            start = mid + 1;
        }
 
        // Decrement end if integral part
        // lies on the left side of the mid
        else {
            end = mid - 1;
        }
    }
 
    // To find the fractional part
    // of square root upto 5 decimal
    fixed increment = 0.1;
    for (int i = 0; i < 5; i++) {
        while (ans * ans <= number) {
            ans += increment;
        }
 
        // Loop terminates,
        // when ans * ans > number
        ans = ans - increment;
        increment = increment / 10;
    }
    return ans;
}

//For these functions to perform correctly fixed point numbers must be in 
//the same format. The format for these functions is 24.8
//Adding two fixed point values together
fixed	fixAdd(fixed a_fa, fixed a_fb)
{
	return a_fa + a_fb;
}
//subtract two fixed point values
fixed	fixSub(fixed a_fa, fixed a_fb)
{
	return a_fa - a_fb;
}
//multiply two fixed point values together
//when multiplying the number of fractional bits needs to be adjusted
//a_fa has FIX_SHIFT decimal bits & a_fb has FIX_SHIFT decimal bits
//this effectively shifts the number up an additional FIX_SHIFT number of bits
//the result must be shifted down by FIX_SHIFT bits
fixed	fixMul(fixed a_fa, fixed a_fb)
{
	return (a_fa * a_fb) / FIX_ONE;
}

fixed	fixDiv(fixed a_fa, fixed a_fb)
{
	return ((a_fa)* FIX_SCALE) / a_fb;
}

s32 lu_sin(u32 theta)
{
	return sin_lut[(theta >> 7) & 0x1FF];
}

s32 lu_cos(u32 theta)
{
	return sin_lut[((theta >> 7) + 128) & 0x1FF];
}

fv2 fv2Create(s32 x, s32 y)
{
	return (fv2){int2fix(x), int2fix(y)};
}

fv2 fvAdd(fv2 fv, fv2 fv1)
{
	fv2 out = { fixAdd(fv.X, fv1.X), fixAdd(fv.Y, fv1.Y) };
	return out;
}

fv2 fvSub(fv2 fv, fv2 fv1)
{
	fv2 out = (fv2){ fixSub(fv.X, fv1.X), fixSub(fv.Y, fv1.Y) };
	return out;
}

fv2 fvMul(fv2 fv, fv2 fv1)
{
	fv2 out = { fixMul(fv.X, fv1.X), fixMul(fv.Y, fv1.Y) };
	return out;
}

fv2 fvDiv(fv2 fv, fv2 fv1)
{
	fv2 out = { fixDiv(fv.X, fv1.X), fixDiv(fv.Y, fv1.Y) };
	return out;
}

fv2 fvNorm(fv2 fv)
{
	fv2 result;
	fixed l = fixSqrt(fv.X * fv.X + fv.Y * fv.Y);
	result.X = fv.X / l;
	result.Y = fv.Y / l;
	return result;
}