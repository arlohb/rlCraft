#ifndef _BLOCK_H
#define _BLOCK_H

#include <raylib.h>

typedef enum {
    AIR,
    GRASS,
    DIRT,
    STONE,
} Block;

Vector2 BlockTexCoords(Block block);

bool IsBlock(Block block);

Block GetBlockFace(Block b1, Block b2);

typedef enum {
    PX,
    NX,
    PY,
    NY,
    PZ,
    NZ,
} Direction;

Vector3 DirectionVector(Direction dir);

#endif
