#include "block.h"

rl::Vector2 BlockTexCoords(Block block) {
    rl::Vector2 pos;

    switch (block) {
        case Block::Air:
            pos = rl::Vector2(0, 0);
            break;
        case Block::Grass:
            pos = rl::Vector2(1, 0);
            break;
        case Block::Dirt:
            pos = rl::Vector2(2, 0);
            break;
        case Block::Stone:
            pos = rl::Vector2(3, 0);
            break;
    }

    return pos / 16.0;
}

rl::Vector3 DirVector(Dir dir) {
    switch (dir) {
    case Dir::Px:
        return rl::Vector3(1, 0, 0);
    case Dir::Nx:
        return rl::Vector3(-1, 0, 0);
    case Dir::Py:
        return rl::Vector3(0, 1, 0);
    case Dir::Ny:
        return rl::Vector3(0, -1, 0);
    case Dir::Pz:
        return rl::Vector3(0, 0, 1);
    case Dir::Nz:
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

