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

#define WIDTH 800
#define HEIGHT 600

#define CHUNKS_X 16
#define CHUNKS_Z 16

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
    for(int x = 0; x < CHUNKS_X; x++)
        for(int z = 0; z < CHUNKS_Z; z++) {
            int i = z * CHUNKS_Z + x;

            chunks[i] = (Chunk){};
            ChunkInit(&chunks[i], (Vector2){ x, z });
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
                            x * CHUNK_WIDTH - (int)(CHUNKS_X * CHUNK_WIDTH / 2),
                            0,
                            z * CHUNK_WIDTH - (int)(CHUNKS_Z * CHUNK_WIDTH / 2),
                        };
                        DrawModel(models[z * CHUNKS_Z + x], pos, 1, WHITE);
                    }

            EndMode3D();

        EndDrawing();
    }

    for(int i = 0; i < CHUNKS_X * CHUNKS_Z; i++)
        // I can't use UnloadModel as that unloads the shared material.
        UnloadMesh(models[i].meshes[0]);
    UnloadMaterial(material);

    CloseWindow();

    return 0;
}
