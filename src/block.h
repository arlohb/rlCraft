#pragma once

#include "rl.h"

enum Block {
    AIR,
    GRASS,
    DIRT,
    STONE,
};

rl::Vector2 BlockTexCoords(Block block);

bool IsBlock(Block block);

Block GetBlockFace(Block b1, Block b2);

enum Direction {
    PX,
    NX,
    PY,
    NY,
    PZ,
    NZ,
};

rl::Vector3 DirectionVector(Direction dir);

