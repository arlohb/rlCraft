#pragma once

#include "types.h"

void DrawTextCodepoint3D(Font font, i32 codepoint, V3 position, f32 fontSize, bool backface, Color tint);
void DrawText3D(Font font, const char *text, V3 position, f32 fontSize, f32 fontSpacing, f32 lineSpacing, bool backface, Color tint);

