#pragma once

#include "rl.h"

class MyCamera {
    public:
        Camera3D camera;
        rl::Vector3 forward;
        rl::Vector3 up;

        float mouseSensitivity;
        float moveSpeed;
        float shiftMod;

        MyCamera();
        void Update();

    private:
        void Rotate();
        void Move();
};

