#pragma once

uint CalculateBezier(Vec4f const *ControlPoints, uint NumControlPoints, Vec4f *Results, int Steps);
uint CalculateNumBezierPoints(uint numControlPoints, uint steps);
