#include "3dText.h"

#include <rlgl.h>

void DrawTextCodepoint3D(Font font, i32 codepoint, V3 position, f32 fontSize, bool backface, Color tint) {
    // Character index position in sprite font
    // NOTE: In case a codepoint is not available in the font, index returned points to '?'
    i32 index = GetGlyphIndex(font, codepoint);
    f32 scale = fontSize / (f32)font.baseSize;

    // Character destination rectangle on screen
    // NOTE: We consider charsPadding on drawing
    position.x += (f32)(font.glyphs[index].offsetX - font.glyphPadding) / (f32)font.baseSize * scale;
    position.z += (f32)(font.glyphs[index].offsetY - font.glyphPadding) / (f32)font.baseSize * scale;

    // Character source rectangle from font texture atlas
    // NOTE: We consider chars padding when drawing, it could be required for outline/glow shader effects
    Rectangle srcRec = {
        font.recs[index].x - (f32)font.glyphPadding,
        font.recs[index].y - (f32)font.glyphPadding,
        font.recs[index].width + 2.0f * font.glyphPadding,
        font.recs[index].height + 2.0f * font.glyphPadding
    };

    f32 width = (f32)(font.recs[index].width + 2.0f * font.glyphPadding) / (f32)font.baseSize * scale;
    f32 height = (f32)(font.recs[index].height + 2.0f * font.glyphPadding) / (f32)font.baseSize * scale;

    if (font.texture.id > 0) {
        const f32 x = 0.0f;
        const f32 y = 0.0f;
        const f32 z = 0.0f;

        // normalized texture coordinates of the glyph inside the font texture (0.0f -> 1.0f)
        const f32 tx = srcRec.x / font.texture.width;
        const f32 ty = srcRec.y / font.texture.height;
        const f32 tw = (srcRec.x + srcRec.width) / font.texture.width;
        const f32 th = (srcRec.y + srcRec.height) / font.texture.height;

        rlCheckRenderBatchLimit(4 + 4 * backface);
        rlSetTexture(font.texture.id);

        rlPushMatrix();
            rlTranslatef(position.x, position.y, position.z);

            rlBegin(RL_QUADS);
                rlColor4ub(tint.r, tint.g, tint.b, tint.a);

                // Front Face
                rlNormal3f(0.0f, 1.0f, 0.0f);                                   // Normal Pointing Up
                rlTexCoord2f(tx, ty); rlVertex3f(x,         y, z);              // Top Left Of The Texture and Quad
                rlTexCoord2f(tx, th); rlVertex3f(x,         y, z + height);     // Bottom Left Of The Texture and Quad
                rlTexCoord2f(tw, th); rlVertex3f(x + width, y, z + height);     // Bottom Right Of The Texture and Quad
                rlTexCoord2f(tw, ty); rlVertex3f(x + width, y, z);              // Top Right Of The Texture and Quad

                if (backface) {
                    // Back Face
                    rlNormal3f(0.0f, -1.0f, 0.0f);                              // Normal Pointing Down
                    rlTexCoord2f(tx, ty); rlVertex3f(x,         y, z);          // Top Right Of The Texture and Quad
                    rlTexCoord2f(tw, ty); rlVertex3f(x + width, y, z);          // Top Left Of The Texture and Quad
                    rlTexCoord2f(tw, th); rlVertex3f(x + width, y, z + height); // Bottom Left Of The Texture and Quad
                    rlTexCoord2f(tx, th); rlVertex3f(x,         y, z + height); // Bottom Right Of The Texture and Quad
                }
            rlEnd();
        rlPopMatrix();

        rlSetTexture(0);
    }
}

void DrawText3D(Font font, const char *text, V3 position, f32 fontSize, f32 fontSpacing, f32 lineSpacing, bool backface, Color tint) {
    i32 length = TextLength(text);          // Total length in bytes of the text, scanned by codepoints in loop

    f32 textOffsetY = 0.0f;               // Offset between lines (on line break '\n')
    f32 textOffsetX = 0.0f;               // Offset X to next character to draw

    f32 scale = fontSize / (f32)font.baseSize;

    for (i32 i = 0; i < length;)
    {
        // Get next codepoint from byte string and glyph index in font
        i32 codepointByteCount = 0;
        i32 codepoint = GetCodepoint(&text[i], &codepointByteCount);
        i32 index = GetGlyphIndex(font, codepoint);

        // NOTE: Normally we exit the decoding sequence as soon as a bad byte is found (and return 0x3f)
        // but we need to draw all of the bad bytes using the '?' symbol moving one byte
        if (codepoint == 0x3f) codepointByteCount = 1;

        if (codepoint == '\n')
        {
            // NOTE: Fixed line spacing of 1.5 line-height
            // TODO: Support custom line spacing defined by user
            textOffsetY += scale + lineSpacing / (f32)font.baseSize * scale;
            textOffsetX = 0.0f;
        }
        else
        {
            if ((codepoint != ' ') && (codepoint != '\t'))
            {
                DrawTextCodepoint3D(font, codepoint, V3(position.x + textOffsetX, position.y, position.z + textOffsetY), fontSize, backface, tint);
            }

            if (font.glyphs[index].advanceX == 0) textOffsetX += (f32)(font.recs[index].width + fontSpacing) / (f32)font.baseSize * scale;
            else textOffsetX += (f32)(font.glyphs[index].advanceX + fontSpacing) / (f32)font.baseSize * scale;
        }

        i += codepointByteCount;   // Move text bytes counter to next codepoint
    }
}
