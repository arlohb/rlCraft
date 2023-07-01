#include <iostream>

#include "3dText.h"
#include "camera.h"
#include "world.h"
#include "noise.h"

const i32 WIDTH = 800;
const i32 HEIGHT = 600;

const bool DEBUG_NOISE = false;

auto DrawGrid() {
    f32 y = 10;
    DrawLine3D(V3(0, y, 0), V3(10, y, 0), RED);
    DrawText3D(GetFontDefault(), "x", V3(10, y, 0), 15, 0, 0, true, RED);
    DrawLine3D(V3(0, y, 0), V3(0, y + 10, 0), GREEN);
    DrawText3D(GetFontDefault(), "y", V3(0, y + 10, 0), 15, 0, 0, true, GREEN);
    DrawLine3D(V3(0, y, 0), V3(0, y, 10), BLUE);
    DrawText3D(GetFontDefault(), "z", V3(0, y, 10), 15, 0, 0, true, BLUE);
}

i32 main() {
    rl::Window window(WIDTH, HEIGHT, "RLCraft");

    MyCamera camera;
    World world;

    if(!DEBUG_NOISE)
        for(i32 x = 0; x < CHUNKS_X; x++)
            for(i32 z = 0; z < CHUNKS_Z; z++)
                world.GenerateChunk(V2(x, z));

    DisableCursor();
    window.SetTargetFPS(60);

    while (!window.ShouldClose()) {
        camera.Update();

        window.BeginDrawing();
            window.ClearBackground(RAYWHITE);

            if (DEBUG_NOISE) {
                FastNoiseLite noise;
                noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
                noise.SetFractalType(FastNoiseLite::FractalType_FBm);
                noise.SetFractalOctaves(4);
                noise.SetFractalLacunarity(1.6);
                noise.SetFractalGain(0.6);
                noise.SetFrequency(0.0025);

                for(i32 x = 0; x < WIDTH; x++)
                    for(i32 y = 0; y < HEIGHT; y++) {
                        f32 value = Remap(noise.GetNoise<f32>(x, y), -1, 1, 0, 1);
                        u8 gray = (u8)(value * 255.0);
                        Color color = rl::Color(gray, gray, gray, 255);
                        DrawPixel(x, y, color);
                    }
            } else {
                BeginMode3D(camera.camera);

                    DrawGrid();

                    world.Draw();

                EndMode3D();
            }

        window.EndDrawing();
    }
}

