#include "block.h"

rl::Vector2 BlockTexCoords(Block block) {
    rl::Vector2 pos;

    switch (block) {
        case AIR:
            pos = rl::Vector2(0, 0);
            break;
        case GRASS:
            pos = rl::Vector2(1, 0);
            break;
        case DIRT:
            pos = rl::Vector2(2, 0);
            break;
        case STONE:
            pos = rl::Vector2(3, 0);
            break;
    }

    return pos / 16.0;
}

rl::Vector3 DirectionVector(Direction dir) {
    switch (dir) {
    case PX:
        return rl::Vector3(1, 0, 0);
    case NX:
        return rl::Vector3(-1, 0, 0);
    case PY:
        return rl::Vector3(0, 1, 0);
    case NY:
        return rl::Vector3(0, -1, 0);
    case PZ:
        return rl::Vector3(0, 0, 1);
    case NZ:
        return rl::Vector3(0, 0, -1);
    }
}

bool IsBlock(Block block) { return block > 0; }

Block GetBlockFace(Block b1, Block b2) {
    if (IsBlock(b1) && !IsBlock(b2)) {
        return b1;
    }
    if (!IsBlock(b1) && IsBlock(b2)) {
        return b2;
    }
    return AIR;
}

