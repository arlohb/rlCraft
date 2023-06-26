#ifndef _CHUNK_H
#define _CHUNK_H

#include "block.h"

#define CHUNK_WIDTH 16
#define CHUNK_HEIGHT 255
#define CHUNK_SIZE CHUNK_WIDTH * CHUNK_WIDTH * CHUNK_HEIGHT

typedef struct {
    // This isn't world coords, its chunk coords.
    Vector2 pos;

    Block* blocks;
} Chunk;

void ChunkInit(Chunk* chunk, Vector2 pos);

Block ChunkGetBlock(Chunk* chunk, int x, int y, int z);
void ChunkSetBlock(Chunk* chunk, int x, int y, int z, Block block);

void ChunkGenerate(Chunk* chunk);

#endif
