#include <stdio.h>
#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
#include <stdlib.h>

#include "3dText.h"
#include "camera.h"
#include "linkedList.h"
#include "chunk.h"
#include "mesh.h"

#define FNL_IMPL
#include "../FastNoiseLite/C/FastNoiseLite.h"

#define WIDTH 800
#define HEIGHT 600

#define CHUNKS_X 16
#define CHUNKS_Z 16

#define DEBUG_NOISE false

int main() {
    printf("%s\n", RAYLIB_VERSION);

    InitWindow(WIDTH, HEIGHT, "RLCraft");

    Texture worldTexture = LoadTexture("assets/Texture.png");
    Shader worldShader = LoadShader("assets/world.vert", "assets/world.frag");
    Material material = LoadMaterialDefault();
    material.maps[MATERIAL_MAP_DIFFUSE].texture = worldTexture;
    material.shader = worldShader;

    Chunk chunks[CHUNKS_X * CHUNKS_Z];
    Model models[CHUNKS_X * CHUNKS_Z];
    if(!DEBUG_NOISE)
        for(int x = 0; x < CHUNKS_X; x++)
            for(int z = 0; z < CHUNKS_Z; z++) {
                int i = z * CHUNKS_Z + x;

                chunks[i] = (Chunk){};
                ChunkInit(&chunks[i], (Vector2){ (float)x, (float)z });
                ChunkGenerate(&chunks[i]);
                models[i] = CreateModel(&chunks[i]);
                models[i].materials[0] = material;
            }

    MyCamera camera = {};
    MyCameraInit(&camera);

    DisableCursor();

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        MyCameraUpdate(&camera);

        BeginDrawing();

            ClearBackground(RAYWHITE);

            if (DEBUG_NOISE) {
                fnl_state noise = fnlCreateState();
                noise.noise_type = FNL_NOISE_OPENSIMPLEX2;
                noise.fractal_type = FNL_FRACTAL_FBM;
                noise.octaves = 4;
                noise.lacunarity = 1.6;
                noise.gain = 0.6;
                noise.frequency = 0.0025;

                for(int x = 0; x < WIDTH; x++)
                    for(int y = 0; y < HEIGHT; y++) {
                        float value = Remap(fnlGetNoise2D(&noise, x, y), -1, 1, 0, 1);
                        unsigned char gray = (unsigned char)(value * 255.0);
                        Color color = (Color){ gray, gray, gray, 255};
                        DrawPixel(x, y, color);
                    }
            } else {
                BeginMode3D(camera.camera);

                    // Draw the axis and labels
                    float y = 10;
                    DrawLine3D((Vector3){ 0, y, 0 }, (Vector3){ 10, y, 0 }, RED);
                    DrawText3D(GetFontDefault(), "x", (Vector3){ 10, y, 0 }, 15, 0, 0, true, RED);
                    DrawLine3D((Vector3){ 0, y, 0 }, (Vector3){ 0, y + 10, 0 }, GREEN);
                    DrawText3D(GetFontDefault(), "y", (Vector3){ 0, y + 10, 0 }, 15, 0, 0, true, GREEN);
                    DrawLine3D((Vector3){ 0, y, 0 }, (Vector3){ 0, y, 10 }, BLUE);
                    DrawText3D(GetFontDefault(), "z", (Vector3){ 0, y, 10 }, 15, 0, 0, true, BLUE);

                    // Draw the world
                    for(int x = 0; x < CHUNKS_X; x++)
                        for(int z = 0; z < CHUNKS_Z; z++) {
                            Vector3 pos = {
                                (float)(x * CHUNK_WIDTH - (int)(CHUNKS_X * CHUNK_WIDTH / 2)),
                                0,
                                (float)(z * CHUNK_WIDTH - (int)(CHUNKS_Z * CHUNK_WIDTH / 2)),
                            };
                            DrawModel(models[z * CHUNKS_Z + x], pos, 1, WHITE);
                        }

                EndMode3D();
            }

        EndDrawing();
    }

    if (!DEBUG_NOISE)
        for(int i = 0; i < CHUNKS_X * CHUNKS_Z; i++)
            // I can't use UnloadModel as that unloads the shared material.
            UnloadMesh(models[i].meshes[0]);
    UnloadMaterial(material);

    CloseWindow();

    return 0;
}
