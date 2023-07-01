#include "chunk.h"
#include "noise.h"

Chunk::Chunk() {
    blocks = new Block[CHUNK_SIZE];

    pos = rl::Vector2::Zero();
}

Chunk::Chunk(Vector2 _pos) {
    blocks = new Block[CHUNK_SIZE];

    pos = _pos;
}

Block Chunk::GetBlock(int x, int y, int z) {
    return blocks[(x * CHUNK_WIDTH * CHUNK_HEIGHT) + (z * CHUNK_HEIGHT) + y];
}

void Chunk::SetBlock(int x, int y, int z, Block block) {
    blocks[(x * CHUNK_WIDTH * CHUNK_HEIGHT) + (z * CHUNK_HEIGHT) + y] = block;
}

struct Noise {
    FastNoiseLite erosion;
    FastNoiseLite cont;
    FastNoiseLite pv;
    FastNoiseLite density;
};

Block GenBlock(Noise& noise, float depth, float squashing, int x, int y, int z) {
    float density = noise.density.GetNoise<float>(x, y, z);
    density = Remap(density, -1, 1, 0, CHUNK_HEIGHT);
    density -= y * squashing;
    density += CHUNK_HEIGHT;

    Block block;

    if (density < depth) {
        block = Block::Air;
    } else if (y == density) {
        block = Block::Dirt;
    } else {
        block = Block::Stone;
    }

    return block;
}

bool RemoveCaves(Noise& noise, int x, int y, int z) {
    float caves = fabsf(noise.pv.GetNoise<float>(x, y, z));
    caves += ((float)y / CHUNK_HEIGHT / 4);
    caves += Remap(Clamp(noise.cont.GetNoise(x * 0.6 + 1000, y * 0.6, z * 0.6), -1, 0), -1, 0, 0.5, 0);

    return caves < 0.2;
}

void Chunk::Generate() {
    Noise noise;

    noise.erosion.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    noise.erosion.SetFractalType(FastNoiseLite::FractalType_FBm);
    noise.erosion.SetFractalOctaves(4);
    noise.erosion.SetFractalLacunarity(1.6);
    noise.erosion.SetFractalGain(0.6);
    noise.erosion.SetFrequency(0.0025);

    noise.cont.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    noise.cont.SetFractalType(FastNoiseLite::FractalType_FBm);
    noise.cont.SetFractalOctaves(5);
    noise.cont.SetFractalLacunarity(1.9);
    noise.cont.SetFractalGain(0.5);
    noise.cont.SetFrequency(0.004);

    noise.pv.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    noise.pv.SetFractalType(FastNoiseLite::FractalType_FBm);
    noise.pv.SetFractalOctaves(3);
    noise.pv.SetFractalLacunarity(1.8);
    noise.pv.SetFractalGain(0.8);
    noise.pv.SetFrequency(0.016);

    noise.density.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    noise.density.SetFractalType(FastNoiseLite::FractalType_FBm);
    noise.density.SetFractalOctaves(5);
    noise.density.SetFractalLacunarity(1.9);
    noise.density.SetFractalGain(0.5);
    noise.density.SetFrequency(0.01);

    for(int cx = 0; cx < CHUNK_WIDTH; cx++)
        for(int cz = 0; cz < CHUNK_WIDTH; cz++) {
            int x = cx + pos.x * CHUNK_WIDTH;
            int z = cz + pos.y * CHUNK_WIDTH;

            float erosion = noise.erosion.GetNoise<float>(x, z);
            float cont = noise.cont.GetNoise<float>(x, z);
            float pv = fabsf(noise.pv.GetNoise<float>(x, z));

            float depth = Remap(cont, -1, 1, 0, CHUNK_HEIGHT) + pv * 25;
            float squashing = Remap(erosion, -1, 1, 1.6, 3.6);

            for(int y = 0; y < CHUNK_HEIGHT; y++) {
                Block block = GenBlock(
                    noise,
                    depth,
                    squashing,
                    x,
                    y,
                    z
                );

                SetBlock(cx, y, cz, block);
            }

            int count = 0;
            for (int y = CHUNK_HEIGHT - 1; y >= 0; y--) {
                if (count >= 5) {
                    break;
                }

                if (GetBlock(cx, y, cz) == Block::Stone) {
                    SetBlock(cx, y, cz, count ? Block::Dirt : Block::Grass);
                    count++;
                }
            }

            for(int y = 0; y < CHUNK_HEIGHT; y++) {
                bool cave = RemoveCaves(noise, x, y, z);

                if (cave) SetBlock(cx, y, cz, Block::Air);
            }
        }
}
