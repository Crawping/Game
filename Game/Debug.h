//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////

void DebugOpen();
void DebugClose();

void DebugSetCamera(Camera *camera);
void DebugEndFrame();

void DebugText(char const *text, ...);
void DebugText(Vec2 const &pos, char const *text, ...);
void DebugText(Vec3 const &pos, char const *text, ...);

void DebugLine(Vec3 const &start, Vec3 const &end, Color color);
void DebugBox(Vec3 const &min, Vec3 const &max, Color color);

//////////////////////////////////////////////////////////////////////
