#include "noise.h"

Noise::Noise() {
    erosion = FastNoiseLite();
    erosion.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    erosion.SetFractalType(FastNoiseLite::FractalType_FBm);
    erosion.SetFractalOctaves(4);
    erosion.SetFractalLacunarity(1.6);
    erosion.SetFractalGain(0.6);
    erosion.SetFrequency(0.0025);

    cont = FastNoiseLite();
    cont.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    cont.SetFractalType(FastNoiseLite::FractalType_FBm);
    cont.SetFractalOctaves(5);
    cont.SetFractalLacunarity(1.9);
    cont.SetFractalGain(0.5);
    cont.SetFrequency(0.004);

    pv = FastNoiseLite();
    pv.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    pv.SetFractalType(FastNoiseLite::FractalType_FBm);
    pv.SetFractalOctaves(3);
    pv.SetFractalLacunarity(1.8);
    pv.SetFractalGain(0.8);
    pv.SetFrequency(0.016);

    density = FastNoiseLite();
    density.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    density.SetFractalType(FastNoiseLite::FractalType_FBm);
    density.SetFractalOctaves(5);
    density.SetFractalLacunarity(1.9);
    density.SetFractalGain(0.5);
    density.SetFrequency(0.01);
}

f32 Noise::Erosion(f32 x, f32 z) {
    return erosion.GetNoise(x, z);
}

f32 Noise::Cont(f32 x, f32 z) {
    return cont.GetNoise(x, z);
}

f32 Noise::Cont3D(f32 x, f32 y, f32 z) {
    return cont.GetNoise(x, y, z);
}

f32 Noise::Pv(f32 x, f32 z) {
    return pv.GetNoise(x, z);
}

f32 Noise::Pv3D(f32 x, f32 y, f32 z) {
    return pv.GetNoise(x, y, z);
}

f32 Noise::Density(f32 x, f32 y, f32 z) {
    return density.GetNoise(x, y, z);
}

