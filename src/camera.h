#pragma once

#include <raylib.h>

class MyCamera {
    public:
        Camera3D camera;
        Vector3 forward;
        Vector3 up;

        float mouseSensitivity;
        float moveSpeed;
        float shiftMod;

        MyCamera();
        void Update();

    private:
        void Rotate();
        void Move();
};

