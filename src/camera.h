#pragma once

#include "types.h"

class MyCamera {
    public:
        Camera3D camera;
        V3 forward;
        V3 up;

        f32 mouseSensitivity;
        f32 moveSpeed;
        f32 shiftMod;

        MyCamera();
        void Update();

    private:
        void Rotate();
        void Move();
};

