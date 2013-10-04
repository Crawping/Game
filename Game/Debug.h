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

void DebugLine(Vec4f const &start, Vec4f const &end, Color color);
void DebugBox(Vec4f const &min, Vec4f const &max, Color color);

//////////////////////////////////////////////////////////////////////
