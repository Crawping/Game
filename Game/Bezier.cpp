//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

//////////////////////////////////////////////////////////////////////

Vector CalculateBezierPoint(float t, Vector p0, Vector p1, Vector p2, Vector const &p3)
{
	float u = 1 - t;
	float tt = t * t;
	float uu = u * u;
	float uuu = uu * u;
	float ttt = tt * t;

	return (uuu * p0) + (3 * uu * t * p1) + (3 * u * tt * p2) + (ttt * p3);
}

//////////////////////////////////////////////////////////////////////
// ControlPoints must be at least 4
// Results must be (ControlPoints * Steps + 1) big

uint CalculateBezier(Vector const *ControlPoints, uint NumControlPoints, Vector *Results, int Steps)
{
	uint o = 0;
	for(uint i=0; i<NumControlPoints-3; i+=3)
	{
		Vector const &p0 = ControlPoints[i + 0];
		Vector const &p1 = ControlPoints[i + 1];
		Vector const &p2 = ControlPoints[i + 2];
		Vector const &p3 = ControlPoints[i + 3];

		if(i == 0)
		{
			Results[o++] = CalculateBezierPoint(0, p0, p1, p2, p3);
		}
		for(int j=1; j <= Steps; ++j)
		{
			float t = j / (float)Steps;
			Results[o++] = CalculateBezierPoint(t, p0, p1, p2, p3);
		}
	}
	return o;
}

//////////////////////////////////////////////////////////////////////

