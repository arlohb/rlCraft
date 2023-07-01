#pragma once

#include "../FastNoiseLite/Cpp/FastNoiseLite.h"
#include "types.h"

class Noise {
    public:
        Noise();

        f32 Erosion(f32 x, f32 z);
        f32 Cont(f32 x, f32 z);
        /// This shouldn't be used during normal world gen.
        /// But currently I'm using it for caves.
        f32 Cont3D(f32 x, f32 y, f32 z);
        f32 Pv(f32 x, f32 z);
        /// This shouldn't be used during normal world gen.
        /// But currently I'm using it for caves.
        f32 Pv3D(f32 x, f32 y, f32 z);
        f32 Density(f32 x, f32 y, f32 z);

    private:
        FastNoiseLite erosion;
        FastNoiseLite cont;
        FastNoiseLite pv;
        FastNoiseLite density;
};

