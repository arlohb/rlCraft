#include "block.h"

Color BlockColour(Block block) {
    switch (block) {
        case AIR:
            return (Color){ 0, 0, 0, 0 };
        case DIRT:
            return (Color){ 125, 67, 36, 255 };
        case STONE:
            return (Color){ 127, 127, 127, 255 };
    }
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

