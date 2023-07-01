#include <iostream>


#include "3dText.h"
#include "camera.h"
#include "chunk.h"
#include "mesh.h"
#include "world.h"
#include "noise.h"

const int WIDTH = 800;
const int HEIGHT = 600;

const bool DEBUG_NOISE = false;

auto DrawGrid() {
    float y = 10;
    DrawLine3D(rl::Vector3(0, y, 0), rl::Vector3(10, y, 0), RED);
    DrawText3D(GetFontDefault(), "x", rl::Vector3(10, y, 0), 15, 0, 0, true, RED);
    DrawLine3D(rl::Vector3(0, y, 0), rl::Vector3(0, y + 10, 0), GREEN);
    DrawText3D(GetFontDefault(), "y", rl::Vector3(0, y + 10, 0), 15, 0, 0, true, GREEN);
    DrawLine3D(rl::Vector3(0, y, 0), rl::Vector3(0, y, 10), BLUE);
    DrawText3D(GetFontDefault(), "z", rl::Vector3(0, y, 10), 15, 0, 0, true, BLUE);
}

int main() {
    rl::Window window(WIDTH, HEIGHT, "RLCraft");

    MyCamera camera;
    World world;

    if(!DEBUG_NOISE)
        for(int x = 0; x < CHUNKS_X; x++)
            for(int z = 0; z < CHUNKS_Z; z++)
                world.GenerateChunk(rl::Vector2(x, z));

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

                for(int x = 0; x < WIDTH; x++)
                    for(int y = 0; y < HEIGHT; y++) {
                        float value = Remap(noise.GetNoise<float>(x, y), -1, 1, 0, 1);
                        unsigned char gray = (unsigned char)(value * 255.0);
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

