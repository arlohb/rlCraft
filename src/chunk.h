#pragma once

#include "block.h"

const i32 CHUNK_WIDTH = 16;
const i32 CHUNK_HEIGHT = 255;
const i32 CHUNK_SIZE = CHUNK_WIDTH * CHUNK_WIDTH * CHUNK_HEIGHT;

class Chunk {
    public:
        V2 pos;

        Chunk();
        Chunk(Vector2 _pos);

        Block GetBlock(i32 x, i32 y, i32 z);
        void SetBlock(i32 x, i32 y, i32 z, Block block);

        void Generate();

    private:
        Block* blocks;
};

