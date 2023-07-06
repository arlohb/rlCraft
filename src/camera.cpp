#include "camera.h"

#include <iostream>

void PrintVector3(V3 v) {
    std::cout << v.x << " " << v.y << " " << v.z << std::endl;
}

MyCamera::MyCamera() {
    SetExitKey(0);

    camera = Camera3D {};
    camera.position = V3(0, 10, 10);
    camera.target = V3::Zero();
    camera.up = V3(0, 1, 0);
    camera.fovy = 70;
    camera.projection = CAMERA_PERSPECTIVE;

    up = V3(0, 1, 0);
    forward = (V3(camera.target) - camera.position).Normalize();

    mouseSensitivity = 0.003;
    moveSpeed = 0.5;
    shiftMod = 2;
}

/// This just changes the forward vector.
/// After this the camera target still needs to be updated.
void MyCamera::Rotate() {
    V3 right = forward.CrossProduct(up);

    V2 delta = V2(GetMouseDelta()) * -mouseSensitivity;

    forward = Vector3RotateByAxisAngle(forward, right, delta.y);
    forward = Vector3RotateByAxisAngle(forward, V3(0, 1, 0), delta.x);
}

/// This changes the camera position,
/// and updates the camera target.
void MyCamera::Move() {
    V3 right = forward.CrossProduct(up);

    V3 movement = V3::Zero();

    if (IsKeyDown(KEY_W)) movement = movement + forward;
    if (IsKeyDown(KEY_S)) movement = movement - forward;
    if (IsKeyDown(KEY_D)) movement = movement + right;
    if (IsKeyDown(KEY_A)) movement = movement - right;
    if (IsKeyDown(KEY_E)) movement = movement + up;
    if (IsKeyDown(KEY_Q)) movement = movement - up;

    if (movement.Length() >= 0.001) {
        f32 movementSpeed = moveSpeed * (IsKeyDown(KEY_LEFT_SHIFT) ? shiftMod : 1);
        movement = movement.Normalize() * movementSpeed;
        camera.position = V3(camera.position) + movement;
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

