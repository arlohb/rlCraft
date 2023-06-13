#include <raylib.h>

#define WIDTH 800
#define HEIGHT 600

int main() {
    InitWindow(WIDTH, HEIGHT, "RLCraft");

    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(RAYWHITE);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
