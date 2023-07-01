#include "chunk.h"

Chunk::Chunk() {
    blocks = new Block[CHUNK_SIZE];

    pos = V2::Zero();
}

Chunk::Chunk(Vector2 _pos) {
    blocks = new Block[CHUNK_SIZE];

    pos = _pos;
}

Block Chunk::GetBlock(i32 x, i32 y, i32 z) {
    return blocks[(x * CHUNK_WIDTH * CHUNK_HEIGHT) + (z * CHUNK_HEIGHT) + y];
}

void Chunk::SetBlock(i32 x, i32 y, i32 z, Block block) {
    blocks[(x * CHUNK_WIDTH * CHUNK_HEIGHT) + (z * CHUNK_HEIGHT) + y] = block;
}

Block GenBlock(Noise& noise, f32 depth, f32 squashing, i32 x, i32 y, i32 z) {
    f32 density = noise.Density(x, y, z);
    density = Remap(density, -1, 1, 0, CHUNK_HEIGHT);
    density -= y * squashing;
    density += CHUNK_HEIGHT;

    Block block;

    if (density < depth) {
        block = Block::Air;
    } else if (y == density) {
        block = Block::Dirt;
    } else {
        block = Block::Stone;
    }

    return block;
}

bool RemoveCaves(Noise& noise, i32 x, i32 y, i32 z) {
    f32 caves = fabsf(noise.Pv3D(x, y, z));
    caves += ((f32)y / CHUNK_HEIGHT / 4);
    caves += Remap(Clamp(noise.Cont3D(x * 0.6 + 1000, y * 0.6, z * 0.6), -1, 0), -1, 0, 0.5, 0);

    return caves < 0.2;
}

void Chunk::Generate(Noise& noise) {
    for(i32 cx = 0; cx < CHUNK_WIDTH; cx++)
        for(i32 cz = 0; cz < CHUNK_WIDTH; cz++) {
            i32 x = cx + pos.x * CHUNK_WIDTH;
            i32 z = cz + pos.y * CHUNK_WIDTH;

            f32 erosion = noise.Erosion(x, z);
            f32 cont = noise.Cont(x, z);
            f32 pv = fabsf(noise.Pv(x, z));

            f32 depth = Remap(cont, -1, 1, 0, CHUNK_HEIGHT) + pv * 25;
            f32 squashing = Remap(erosion, -1, 1, 1.6, 3.6);

            for(i32 y = 0; y < CHUNK_HEIGHT; y++) {
                Block block = GenBlock(
                    noise,
                    depth,
                    squashing,
                    x,
                    y,
                    z
                );

                SetBlock(cx, y, cz, block);
            }

            i32 count = 0;
            for (i32 y = CHUNK_HEIGHT - 1; y >= 0; y--) {
                if (count >= 5) {
                    break;
                }

                if (GetBlock(cx, y, cz) == Block::Stone) {
                    SetBlock(cx, y, cz, count ? Block::Dirt : Block::Grass);
                    count++;
                }
            }

            for(i32 y = 0; y < CHUNK_HEIGHT; y++) {
                bool cave = RemoveCaves(noise, x, y, z);

                if (cave) SetBlock(cx, y, cz, Block::Air);
            }
        }
}
