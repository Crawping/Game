//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

//////////////////////////////////////////////////////////////////////

static void CalculateBezierPoint(Vec2 &result, float t, Vec2 const &p0, Vec2 const &p1, Vec2 const &p2, Vec2 const &p3)
{
	float u = 1 - t;
	float tt = t * t;
	float uu = u * u;
	float uuu = uu * u;
	float ttt = tt * t;

	result = uuu * p0;
	result += 3 * uu * t * p1;
	result += 3 * u * tt * p2;
	result += ttt * p3;
}

//////////////////////////////////////////////////////////////////////
// ControlPoints must be at least 4
// Results must be (ControlPoints * Steps + 1) big

uint CalculateBezier(Vec2 const *ControlPoints, uint NumControlPoints, Vec2 *Results, int Steps)
{
	uint o = 0;
	for(uint i=0; i<NumControlPoints-3; i+=3)
	{
		Vec2 const &p0 = ControlPoints[i + 0];
		Vec2 const &p1 = ControlPoints[i + 1];
		Vec2 const &p2 = ControlPoints[i + 2];
		Vec2 const &p3 = ControlPoints[i + 3];

		if(i == 0)
		{
			CalculateBezierPoint(Results[o++], 0, p0, p1, p2, p3);
		}
		for(int j=1; j <= Steps; ++j)
		{
			float t = j / (float)Steps;
			CalculateBezierPoint(Results[o++], t, p0, p1, p2, p3);
		}
	}
	return o;
}

//////////////////////////////////////////////////////////////////////

