#include "block.h"

rl::Vector2 BlockTexCoords(Block block) {
    rl::Vector2 pos;

    switch (block) {
        using enum Block;
        case Air:
            pos = rl::Vector2(0, 0);
            break;
        case Grass:
            pos = rl::Vector2(1, 0);
            break;
        case Dirt:
            pos = rl::Vector2(2, 0);
            break;
        case Stone:
            pos = rl::Vector2(3, 0);
            break;
    }

    return pos / 16.0;
}

rl::Vector3 DirVector(Dir dir) {
    switch (dir) {
        using enum Dir;
        case Px:
            return rl::Vector3(1, 0, 0);
        case Nx:
            return rl::Vector3(-1, 0, 0);
        case Py:
            return rl::Vector3(0, 1, 0);
        case Ny:
            return rl::Vector3(0, -1, 0);
        case Pz:
            return rl::Vector3(0, 0, 1);
        case Nz:
            return rl::Vector3(0, 0, -1);
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

