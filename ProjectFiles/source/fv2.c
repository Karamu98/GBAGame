#include "fv2.h"

fv2 fv2Create(fixed a_x, fixed a_y)
{
	fv2 out = { a_x, a_y };
	return out;
}

/*u8 DistanceBetween(fv2 a_a, fv2 a_b)
{
	u8 dx = a_a.fX - a_b.fX;
	u8 dy = a_a.fX - a_b.fY;

	return sqrt(dx * dx + dy * dy);
}*/

fv2 fvAdd(fv2 fv, fv2 fv1)
{
	fv2 out = { fixAdd(fv.fX, fv1.fX), fixAdd(fv.fY, fv1.fY) };
	return out;
}

fv2 fvSub(fv2 fv, fv2 fv1)
{
	fv2 out = { fixSub(fv.fX, fv1.fX), fixSub(fv.fY, fv1.fY) };
	return out;
}

fv2 fvMul(fv2 fv, fv2 fv1)
{
	fv2 out = { fixMul(fv.fX, fv1.fX), fixMul(fv.fY, fv1.fY) };
	return out;
}

fv2 fvDiv(fv2 fv, fv2 fv1)
{
	fv2 out = { fixDiv(fv.fX, fv1.fX), fixDiv(fv.fY, fv1.fY) };
	return out;
}

fv2 fvInvert(fv2 fv)
{
	fv2 inverse;
	inverse = fv2Create(int2fix(-fix2int(fv.fX)), int2fix(-fix2int(fv.fY)));
	return inverse;
}
