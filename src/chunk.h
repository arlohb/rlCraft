#pragma once

#include "block.h"

#define CHUNK_WIDTH 16
#define CHUNK_HEIGHT 255
#define CHUNK_SIZE CHUNK_WIDTH * CHUNK_WIDTH * CHUNK_HEIGHT

class Chunk {
    public:
        rl::Vector2 pos;

        Chunk();
        Chunk(Vector2 _pos);

        Block GetBlock(int x, int y, int z);
        void SetBlock(int x, int y, int z, Block block);

        void Generate();

    private:
        Block* blocks;
};

