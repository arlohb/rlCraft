#pragma once

#include <raylib.h>

typedef struct {
    Camera3D camera;
    Vector3 forward;
    Vector3 up;

    float mouseSensitivity;
    float moveSpeed;
    float shiftMod;
} MyCamera;

void MyCameraInit(MyCamera* camera);

void MyCameraUpdate(MyCamera* camera);

