#pragma once

#include <raylib.h>

struct MyCamera {
    Camera3D camera;
    Vector3 forward;
    Vector3 up;

    float mouseSensitivity;
    float moveSpeed;
    float shiftMod;
};

void MyCameraInit(MyCamera* camera);

void MyCameraUpdate(MyCamera* camera);

