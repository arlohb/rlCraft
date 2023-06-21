#ifndef _CHUNK_H
#define _CHUNK_H

#include "block.h"

#define CHUNK_WIDTH 16
#define CHUNK_HEIGHT 255
#define CHUNK_SIZE CHUNK_WIDTH * CHUNK_WIDTH * CHUNK_HEIGHT

typedef struct {
    Block* blocks;
} Chunk;

void ChunkInit(Chunk* chunk);

Block ChunkGetBlock(Chunk* chunk, int x, int y, int z);
void ChunkSetBlock(Chunk* chunk, int x, int y, int z, Block block);

Block ChunkGenBlock(int x, int y, int z);
void ChunkGenerate(Chunk* chunk);

#endif