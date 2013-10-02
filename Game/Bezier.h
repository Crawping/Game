#pragma once

uint CalculateBezier(Vector const *ControlPoints, uint NumControlPoints, Vector *Results, int Steps);
uint CalculateNumBezierPoints(uint numControlPoints, uint steps);
