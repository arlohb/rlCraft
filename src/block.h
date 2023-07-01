#pragma once

#include "types.h"

enum class Block {
    Air,
    Grass,
    Dirt,
    Stone,
};

V2 BlockTexCoords(Block block);

bool IsBlock(Block block);

Block GetBlockFace(Block b1, Block b2);

enum class Dir {
    Px,
    Nx,
    Py,
    Ny,
    Pz,
    Nz,
};

V3 DirVector(Dir dir);

