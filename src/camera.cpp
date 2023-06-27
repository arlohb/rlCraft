#include "camera.h"

#include <iostream>
#include "raymath.h"

void PrintVector3(Vector3 v) {
    std::cout << v.x << " " << v.y << " " << v.z << std::endl;
}

MyCamera::MyCamera() {
    SetExitKey(0);

    camera = (Camera3D){};
    camera.position = (Vector3){ 0, 10, 10 };
    camera.target = Vector3Zero();
    camera.up = (Vector3){ 0, 1, 0 };
    camera.fovy = 70;
    camera.projection = CAMERA_PERSPECTIVE;

    up = (Vector3){ 0, 1, 0 };
    forward = Vector3Normalize(Vector3Subtract(camera.target, camera.position));

    mouseSensitivity = 0.003;
    moveSpeed = 0.5;
    shiftMod = 2;
}

/// This just changes the forward vector.
/// After this the camera target still needs to be updated.
void MyCamera::Rotate() {
    Vector3 right = Vector3CrossProduct(forward, up);

    Vector2 delta = Vector2Scale(GetMouseDelta(), -mouseSensitivity);

    forward = Vector3RotateByAxisAngle(forward, right, delta.y);
    forward = Vector3RotateByAxisAngle(forward, (Vector3){ 0, 1, 0 }, delta.x);
}

/// This changes the camera position,
/// and updates the camera target.
void MyCamera::Move() {
    Vector3 right = Vector3CrossProduct(forward, up);

    Vector3 movement = Vector3Zero();

    if (IsKeyDown(KEY_W)) movement = Vector3Add(movement, forward);
    if (IsKeyDown(KEY_S)) movement = Vector3Subtract(movement, forward);
    if (IsKeyDown(KEY_D)) movement = Vector3Add(movement, right);
    if (IsKeyDown(KEY_A)) movement = Vector3Subtract(movement, right);
    if (IsKeyDown(KEY_E)) movement = Vector3Add(movement, up);
    if (IsKeyDown(KEY_Q)) movement = Vector3Subtract(movement, up);

    if (Vector3LengthSqr(movement) >= 0.001) {
        float movementSpeed = moveSpeed * (IsKeyDown(KEY_LEFT_SHIFT) ? shiftMod : 1);
        movement = Vector3Scale(Vector3Normalize(movement), movementSpeed);
        camera.position = Vector3Add(camera.position, movement);
    }

    camera.target = Vector3Add(camera.position, forward);
}

void MyCamera::Update() {
    // Free the cursor if escape is pressed
    if (IsKeyDown(KEY_ESCAPE) || IsKeyDown(KEY_CAPS_LOCK)) EnableCursor();

    // Lock the cursor when pressed
    if (!IsCursorHidden() && IsMouseButtonDown(0)) DisableCursor();

    // Only update camera if mouse locked
    if (!IsCursorHidden()) return;

    Rotate();
    Move();
}

