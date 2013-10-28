//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////

void DebugOpen();
void DebugClose();

void DebugSetCamera(Camera *camera);
void DebugEndFrame();

void DebugText(char const *text, ...);
void DebugText(Vec2 const &pos, char const *text, ...);
void DebugText(Point2D const &pos, char const *text, ...);
void DebugText(int x, int y, char const *text, ...);
void DebugText(Vec4f const &pos, char const *text, ...);

void DebugBeginLines();
void DebugLine(Vec4f const &start, Vec4f const &end, Color color);
void DebugEndLines();

void DebugOneLine(Vec4f const &start, Vec4f const &end, Color color);

void DebugBox(Vec4f const &min, Vec4f const &max, Color color);
void DebugSphere(Vec4f const &pos, float radius, Color color, int segments = 1, int smoothness = 16);
void DebugCylinder(Vec4f const &pos, float radius, float width, Color color, int segments = 2, int smoothness = 16);
void DebugAxes(Vec4f const &pos, float length);

//////////////////////////////////////////////////////////////////////
