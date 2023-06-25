#include "camera.h"

#include <stdio.h>
#include "raymath.h"

void PrintVector3(Vector3 v) {
    printf("%f, %f, %f\n", v.x, v.y, v.z);
}

void MyCameraInit(MyCamera *camera) {
    SetExitKey(0);

    camera->camera = (Camera3D){};
    camera->camera.position = (Vector3){ 0, 10, 10 };
    camera->camera.target = Vector3Zero();
    camera->camera.up = (Vector3){ 0, 1, 0 };
    camera->camera.fovy = 70;
    camera->camera.projection = CAMERA_PERSPECTIVE;

    camera->up = (Vector3){ 0, 1, 0 };
    camera->forward = Vector3Normalize(Vector3Subtract(camera->camera.target, camera->camera.position));

    camera->mouseSensitivity = 0.003;
    camera->moveSpeed = 0.5;
    camera->shiftMod = 2;
}

/// This just changes the forward vector.
/// After this the camera target still needs to be updated.
void Rotate(MyCamera* camera) {
    Vector3 right = Vector3CrossProduct(camera->forward, camera->up);

    Vector2 delta = Vector2Scale(GetMouseDelta(), -camera->mouseSensitivity);

    camera->forward = Vector3RotateByAxisAngle(camera->forward, right, delta.y);
    camera->forward = Vector3RotateByAxisAngle(camera->forward, (Vector3){ 0, 1, 0 }, delta.x);
}

/// This changes the camera position,
/// and updates the camera target.
void Movement(MyCamera* camera) {
    Vector3 right = Vector3CrossProduct(camera->forward, camera->up);

    Vector3 movement = Vector3Zero();

    if (IsKeyDown(KEY_W)) movement = Vector3Add(movement, camera->forward);
    if (IsKeyDown(KEY_S)) movement = Vector3Subtract(movement, camera->forward);
    if (IsKeyDown(KEY_D)) movement = Vector3Add(movement, right);
    if (IsKeyDown(KEY_A)) movement = Vector3Subtract(movement, right);
    if (IsKeyDown(KEY_E)) movement = Vector3Add(movement, camera->up);
    if (IsKeyDown(KEY_Q)) movement = Vector3Subtract(movement, camera->up);

    if (Vector3LengthSqr(movement) >= 0.001) {
        float movementSpeed = camera->moveSpeed * (IsKeyDown(KEY_LEFT_SHIFT) ? camera->shiftMod : 1);
        movement = Vector3Scale(Vector3Normalize(movement), movementSpeed);
        camera->camera.position = Vector3Add(camera->camera.position, movement);
    }

    camera->camera.target = Vector3Add(camera->camera.position, camera->forward);
}

void MyCameraUpdate(MyCamera* camera) {
    // Free the cursor if escape is pressed
    if (IsKeyDown(KEY_ESCAPE) || IsKeyDown(KEY_CAPS_LOCK)) EnableCursor();

    // Lock the cursor when pressed
    if (!IsCursorHidden() && IsMouseButtonDown(0)) DisableCursor();

    // Only update camera if mouse locked
    if (!IsCursorHidden()) return;

    Rotate(camera);
    Movement(camera);
}

