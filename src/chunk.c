#include "chunk.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

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

Block ChunkGenBlock(int x, int y, int z) {
    int height = CHUNK_HEIGHT / 2 + (int)(4 * sin(x / 4.0)) + (int)(4 * sin(z / 4.0));
    Block block;

    if (y > height) {
        block = AIR;
    } else if (y == height) {
        block = DIRT;
    } else {
        block = STONE;
    }

    return block;
}

void ChunkGenerate(Chunk* chunk) {
    for(int x = 0; x < CHUNK_WIDTH; x++)
        for(int z = 0; z < CHUNK_WIDTH; z++)
            for(int y = 0; y < CHUNK_HEIGHT; y++) {
                Block block = ChunkGenBlock(
                    x + chunk->pos.x * CHUNK_WIDTH,
                    y,
                    z + chunk->pos.y * CHUNK_WIDTH
                );

                ChunkSetBlock(chunk, x, y, z, block);
            }
}
