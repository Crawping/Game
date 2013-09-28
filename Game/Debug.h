//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////

void DebugOpen();
void DebugClose();

void DebugSetCamera(Camera *camera);
void DebugEndFrame();

void DebugText(char const *text, ...);
void DebugText(Vec2 const &pos, char const *text, ...);
void DebugText(Vec4 const &pos, char const *text, ...);

void DebugLine(Vec4 const &start, Vec4 const &end, Color color);
void DebugBox(Vec4 const &min, Vec4 const &max, Color color);

//////////////////////////////////////////////////////////////////////
