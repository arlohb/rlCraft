#include "block.h"

V2 BlockTexCoords(Block block) {
    V2 pos;

    switch (block) {
        using enum Block;
        case Air:
            pos = V2(0, 0);
            break;
        case Grass:
            pos = V2(1, 0);
            break;
        case Dirt:
            pos = V2(2, 0);
            break;
        case Stone:
            pos = V2(3, 0);
            break;
    }

    return pos / 16.0;
}

V3 DirVector(Dir dir) {
    switch (dir) {
        using enum Dir;
        case Px:
            return V3(1, 0, 0);
        case Nx:
            return V3(-1, 0, 0);
        case Py:
            return V3(0, 1, 0);
        case Ny:
            return V3(0, -1, 0);
        case Pz:
            return V3(0, 0, 1);
        case Nz:
            return V3(0, 0, -1);
    }
}

bool IsBlock(Block block) { return block != Block::Air; }

Block GetBlockFace(Block b1, Block b2) {
    if (IsBlock(b1) && !IsBlock(b2)) {
        return b1;
    }
    if (!IsBlock(b1) && IsBlock(b2)) {
        return b2;
    }
    return Block::Air;
}

