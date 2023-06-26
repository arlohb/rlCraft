#include "chunk.h"
#include "raymath.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "../FastNoiseLite/C/FastNoiseLite.h"

void ChunkInit(Chunk* chunk, Vector2 pos) {
    // This *might* not be necessary to fill with 0s,
    // but I'll do it just in case.
    chunk->blocks = calloc(CHUNK_SIZE, sizeof(Block));

    chunk->pos = pos;
}

Block ChunkGetBlock(Chunk* chunk, int x, int y, int z) {
    return chunk->blocks[(x * CHUNK_WIDTH * CHUNK_HEIGHT) + (z * CHUNK_HEIGHT) + y];
}

void ChunkSetBlock(Chunk* chunk, int x, int y, int z, Block block) {
    chunk->blocks[(x * CHUNK_WIDTH * CHUNK_HEIGHT) + (z * CHUNK_HEIGHT) + y] = block;
}

typedef struct {
    fnl_state erosion;
    fnl_state cont;
    fnl_state pv;
    fnl_state density;
} Noise;

Block ChunkGenBlock(Noise* noise, float depth, float squashing, int x, int y, int z) {
    float density = fnlGetNoise3D(&noise->density, x, y, z);
    density = Remap(density, -1, 1, 0, CHUNK_HEIGHT);
    density -= y * squashing;
    density += CHUNK_HEIGHT;

    Block block;

    if (density < depth) {
        block = AIR;
    } else if (y == density) {
        block = DIRT;
    } else {
        block = STONE;
    }

    return block;
}

bool RemoveCaves(Noise* noise, int x, int y, int z) {
    float caves = fabsf(fnlGetNoise3D(&noise->pv, x, y, z));
    caves += ((float)y / CHUNK_HEIGHT / 4);
    caves += Remap(Clamp(fnlGetNoise3D(&noise->cont, x * 0.6 + 1000, y * 0.6, z * 0.6), -1, 0), -1, 0, 0.5, 0);

    return caves < 0.2;
}

void ChunkGenerate(Chunk* chunk) {
    Noise noise = {
        .erosion = fnlCreateState(),
        .cont = fnlCreateState(),
        .pv = fnlCreateState(),
        .density = fnlCreateState(),
    };

    noise.erosion.noise_type = FNL_NOISE_OPENSIMPLEX2;
    noise.erosion.fractal_type = FNL_FRACTAL_FBM;
    noise.erosion.octaves = 4;
    noise.erosion.lacunarity = 1.6;
    noise.erosion.gain = 0.6;
    noise.erosion.frequency = 0.0025;

    noise.cont.noise_type = FNL_NOISE_OPENSIMPLEX2;
    noise.cont.fractal_type = FNL_FRACTAL_FBM;
    noise.cont.octaves = 5;
    noise.cont.lacunarity = 1.9;
    noise.cont.gain = 0.5;
    noise.cont.frequency = 0.004;

    noise.pv.noise_type = FNL_NOISE_OPENSIMPLEX2;
    noise.pv.fractal_type = FNL_FRACTAL_FBM;
    noise.pv.octaves = 3;
    noise.pv.lacunarity = 1.8;
    noise.pv.gain = 0.8;
    noise.pv.frequency = 0.016;

    noise.density.noise_type = FNL_NOISE_OPENSIMPLEX2;
    noise.density.fractal_type = FNL_FRACTAL_FBM;
    noise.density.octaves = 5;
    noise.density.lacunarity = 1.9;
    noise.density.gain = 0.5;
    noise.density.frequency = 0.01;

    for(int cx = 0; cx < CHUNK_WIDTH; cx++)
        for(int cz = 0; cz < CHUNK_WIDTH; cz++) {
            int x = cx + chunk->pos.x * CHUNK_WIDTH;
            int z = cz + chunk->pos.y * CHUNK_WIDTH;

            float erosion = fnlGetNoise2D(&noise.erosion, x, z);
            float cont = fnlGetNoise2D(&noise.cont, x, z);
            float pv = fabsf(fnlGetNoise2D(&noise.pv, x, z));

            float depth = Remap(cont, -1, 1, 0, CHUNK_HEIGHT) + pv * 25;
            float squashing = Remap(erosion, -1, 1, 1.6, 3.6);

            for(int y = 0; y < CHUNK_HEIGHT; y++) {
                Block block = ChunkGenBlock(
                    &noise,
                    depth,
                    squashing,
                    x,
                    y,
                    z
                );

                ChunkSetBlock(chunk, cx, y, cz, block);
            }

            int count = 0;
            for (int y = CHUNK_HEIGHT - 1; y >= 0; y--) {
                if (count >= 5) {
                    break;
                }

                if (ChunkGetBlock(chunk, cx, y, cz) == STONE) {
                    ChunkSetBlock(chunk, cx, y, cz, count ? DIRT : GRASS);
                    count++;
                }
            }

            for(int y = 0; y < CHUNK_HEIGHT; y++) {
                bool cave = RemoveCaves(&noise, x, y, z);

                if (cave) ChunkSetBlock(chunk, cx, y, cz, AIR);
            }
        }
}
