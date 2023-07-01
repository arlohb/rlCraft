#pragma once

#include "block.h"

const int CHUNK_WIDTH = 16;
const int CHUNK_HEIGHT = 255;
const int CHUNK_SIZE = CHUNK_WIDTH * CHUNK_WIDTH * CHUNK_HEIGHT;

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

