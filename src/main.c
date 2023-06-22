#include <stdio.h>
#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
#include <stdlib.h>

#include "3dText.h"
#include "linkedList.h"
#include "chunk.h"
#include "mesh.h"

#define WIDTH 800
#define HEIGHT 600

int main() {
    InitWindow(WIDTH, HEIGHT, "RLCraft");
    rlDisableBackfaceCulling();

    Chunk chunk;
    ChunkInit(&chunk);
    ChunkGenerate(&chunk);

    Model model = CreateModel(&chunk);

    Camera3D camera = {};
    camera.position = (Vector3) { 0, 10, 10 };
    camera.target = Vector3Zero();
    camera.up = (Vector3) { 0, 1, 0 };
    camera.fovy = 70;
    camera.projection = CAMERA_PERSPECTIVE;
    SetCameraMode(camera, CAMERA_FREE);

    SetTargetFPS(60);

    Texture worldTexture = LoadTexture("assets/Texture.png");
    model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = worldTexture;

    Shader worldShader = LoadShader(0, "assets/world.frag");
    model.materials[0].shader = worldShader;

    while (!WindowShouldClose()) {
        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(camera);

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

                UpdateCamera(&camera);

            EndMode3D();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
