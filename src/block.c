#include "block.h"

#include "raymath.h"

Vector2 BlockTexCoords(Block block) {
    Vector2 pos;

    switch (block) {
        case AIR:
            pos = (Vector2){ 0, 0 };
            break;
        case GRASS:
            pos = (Vector2){ 1, 0 };
            break;
        case DIRT:
            pos = (Vector2){ 2, 0 };
            break;
        case STONE:
            pos = (Vector2){ 3, 0 };
            break;
    }

    return Vector2Scale(pos, 1.0 / 16.0);
}

Vector3 DirectionVector(Direction dir) {
    switch (dir) {
    case PX:
        return (Vector3){1, 0, 0};
    case NX:
        return (Vector3){-1, 0, 0};
    case PY:
        return (Vector3){0, 1, 0};
    case NY:
        return (Vector3){0, -1, 0};
    case PZ:
        return (Vector3){0, 0, 1};
    case NZ:
        return (Vector3){0, 0, -1};
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

