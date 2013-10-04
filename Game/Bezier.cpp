//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

//////////////////////////////////////////////////////////////////////

Vec4f CalculateBezierPoint(float t, Vec4f p0, Vec4f p1, Vec4f p2, Vec4f const &p3)
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

uint CalculateBezier(Vec4f const *ControlPoints, uint NumControlPoints, Vec4f *Results, int Steps)
{
	uint o = 0;
	for(uint i=0; i<NumControlPoints-3; i+=3)
	{
		Vec4f const &p0 = ControlPoints[i + 0];
		Vec4f const &p1 = ControlPoints[i + 1];
		Vec4f const &p2 = ControlPoints[i + 2];
		Vec4f const &p3 = ControlPoints[i + 3];

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

uint CalculateNumBezierPoints(uint numControlPoints, uint steps)
{
	int o = 0;
	for(uint i=0; i<numControlPoints-3; i+=3)
	{
		if(i == 0)
		{
			++o;
		}
		for(uint j=1; j<=steps; ++j)
		{
			++o;
		}
	}
	int n = (numControlPoints / 3) * steps + 1;
	return n;
}