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

int main() {
    printf("%s\n", RAYLIB_VERSION);

    InitWindow(WIDTH, HEIGHT, "RLCraft");
    rlDisableBackfaceCulling();

    Chunk chunk;
    ChunkInit(&chunk);
    ChunkGenerate(&chunk);

    Model model = CreateModel(&chunk);

    MyCamera camera = {};
    MyCameraInit(&camera);

    DisableCursor();

    SetTargetFPS(60);

    Texture worldTexture = LoadTexture("assets/Texture.png");
    model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = worldTexture;

    Shader worldShader = LoadShader("assets/world.vert", "assets/world.frag");
    model.materials[0].shader = worldShader;

    while (!WindowShouldClose()) {
        MyCameraUpdate(&camera);

        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(camera.camera);

                // Draw the grid, axis, and axis labels
                DrawGrid(10, 1);
                DrawLine3D(Vector3Zero(), (Vector3){ 10, 0, 0 }, RED);
                DrawText3D(GetFontDefault(), "x", (Vector3){ 10, 0, 0 }, 15, 0, 0, true, RED);
                DrawLine3D(Vector3Zero(), (Vector3){ 0, 10, 0 }, GREEN);
                DrawText3D(GetFontDefault(), "y", (Vector3){ 0, 10, 0 }, 15, 0, 0, true, GREEN);
                DrawLine3D(Vector3Zero(), (Vector3){ 0, 0, 10 }, BLUE);
                DrawText3D(GetFontDefault(), "z", (Vector3){ 0, 0, 10 }, 15, 0, 0, true, BLUE);

                // Draw the model
                DrawModel(model, Vector3Zero(), 1, WHITE);

                // Draw the model vertices
                if (false)
                    for(int i = 0; i < model.meshes[0].vertexCount; i++) {
                        Vector3 pos = {
                            model.meshes[0].vertices[i * 3],
                            model.meshes[0].vertices[i * 3 + 1],
                            model.meshes[0].vertices[i * 3 + 2],
                        };

                        DrawSphere(pos, 0.1, RED);
                    }

            EndMode3D();

        EndDrawing();
    }

    UnloadTexture(worldTexture);
    UnloadShader(worldShader);
    UnloadModel(model);

    CloseWindow();

    return 0;
}
