//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////

void DebugOpen();
void DebugClose();

void DebugSetCamera(Camera *camera);
void DebugEndFrame();

void DebugText(char const *text, ...);
void DebugText(Vec2 const &pos, char const *text, ...);
void DebugText(Vector const &pos, char const *text, ...);

void DebugLine(Vector const &start, Vector const &end, Color color);
void DebugBox(Vector const &min, Vector const &max, Color color);

//////////////////////////////////////////////////////////////////////
