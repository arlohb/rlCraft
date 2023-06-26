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
} Noise;

Block ChunkGenBlock(Noise* noise, int x, int y, int z) {
    float erosionVal = fnlGetNoise2D(&noise->erosion, x, y);

    int density = Remap(fnlGetNoise3D(&noise->cont, x, y, z), -1, 1, 0, CHUNK_HEIGHT);
    float squash = 0.8;
    density += fabsf(fnlGetNoise3D(&noise->pv, x, y, z)) * 25.0;
    density -= erosionVal * CHUNK_HEIGHT * squash / 2;
    density -= y;
    density += 127;

    Block block;

    if (y > density) {
        block = AIR;
    } else if (y == density) {
        block = DIRT;
    } else {
        block = STONE;
    }

    float scale = 1.0;
    float caves = fabsf(fnlGetNoise3D(&noise->pv, x * scale, y * scale, z * scale));
    caves += ((float)y / CHUNK_HEIGHT / 4);
    caves += Remap(Clamp(fnlGetNoise3D(&noise->cont, x * 0.6 + 1000, y * 0.6, z * 0.6), -1, 0), -1, 0, 0.5, 0);

    if (caves < 0.2) {
        block = AIR;
    }

    return block;
}

void ChunkGenerate(Chunk* chunk) {
    Noise noise = {
        .erosion = fnlCreateState(),
        .cont = fnlCreateState(),
        .pv = fnlCreateState(),
    };

    noise.erosion.noise_type = FNL_NOISE_OPENSIMPLEX2;
    noise.erosion.fractal_type = FNL_FRACTAL_FBM;
    noise.erosion.octaves = 4;
    noise.erosion.lacunarity = 1.4;
    noise.erosion.gain = 0.9;
    noise.erosion.frequency = 0.003;

    noise.cont.noise_type = FNL_NOISE_OPENSIMPLEX2;
    noise.cont.fractal_type = FNL_FRACTAL_FBM;
    noise.cont.octaves = 5;
    noise.cont.lacunarity = 1.5;
    noise.cont.gain = 0.4;
    noise.cont.frequency = 0.008;

    noise.pv.noise_type = FNL_NOISE_OPENSIMPLEX2;
    noise.pv.fractal_type = FNL_FRACTAL_FBM;
    noise.pv.octaves = 3;
    noise.pv.lacunarity = 1.8;
    noise.pv.gain = 0.8;
    noise.pv.frequency = 0.016;

    for(int x = 0; x < CHUNK_WIDTH; x++)
        for(int z = 0; z < CHUNK_WIDTH; z++)
            for(int y = 0; y < CHUNK_HEIGHT; y++) {
                Block block = ChunkGenBlock(
                    &noise,
                    x + chunk->pos.x * CHUNK_WIDTH,
                    y,
                    z + chunk->pos.y * CHUNK_WIDTH
                );

                ChunkSetBlock(chunk, x, y, z, block);
            }
}
