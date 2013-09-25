//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DirectXMath.h"

using namespace DirectX;

//////////////////////////////////////////////////////////////////////

Matrix IdentityMatrix(1,0,0,0,
					  0,1,0,0,
					  0,0,1,0,
					  0,0,0,1);

//////////////////////////////////////////////////////////////////////

Vec3 GetYawPitchRollFromMatrix(Matrix const &m)
{
	MatrixArray &a = (MatrixArray &)*(&m);
	return Vec3( asinf(a[1][0]), atan2f(-a[1][2], a[1][1]), atan2f(-a[2][0], a[0][0]) );
}
