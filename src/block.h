#pragma once

#include "rl.h"

enum class Block {
    Air,
    Grass,
    Dirt,
    Stone,
};

rl::Vector2 BlockTexCoords(Block block);

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

rl::Vector3 DirVector(Dir dir);

