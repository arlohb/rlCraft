#include <stdio.h>
#include <raylib.h>
#include <raymath.h>

#define WIDTH 800
#define HEIGHT 600

int main() {
    InitWindow(WIDTH, HEIGHT, "RLCraft");

    Camera3D camera = {};
    camera.position = (Vector3) { 0, 10, 10 };
    camera.target = Vector3Zero();
    camera.up = (Vector3) { 0, 1, 0 };
    camera.fovy = 45;
    camera.projection = CAMERA_PERSPECTIVE;
    SetCameraMode(camera, CAMERA_FREE);

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(camera);

                DrawCube(Vector3Zero(), 2, 2, 2, RED);

                DrawGrid(10, 1);

                UpdateCamera(&camera);

            EndMode3D();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
