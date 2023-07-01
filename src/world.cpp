#include "world.h"

#include <chrono>
#include <iostream>
#include "mesh.h"

World::World() {
    worldTex = rl::Texture("assets/Texture.png");
    worldShader = LoadShader("assets/world.vert", "assets/world.frag");
    worldMat.maps[MATERIAL_MAP_DIFFUSE].texture = worldTex;
    worldMat.shader = worldShader;
}

World::~World() {
    for (auto [_, model] : models)
        // I can't use UnloadModel as that unloads the shared material.
        UnloadMesh(model.meshes[0]);
}

void World::GenerateChunk(V2 v) {
    Chunk chunk;
    chunk.pos = v;
    chunk.Generate(noise);
    chunks.insert(std::pair<V2, Chunk>(v, chunk));

    Model model = CreateModel(chunk);
    model.materials[0] = worldMat;
    models.insert(std::pair<V2, Model>(v, model));
}

void World::InitialGeneration() {
    auto start = std::chrono::system_clock::now();

    int i = 0;
    const int max = CHUNKS_X * CHUNKS_Z;
    const int width = 80;

    for(i32 x = 0; x < CHUNKS_X; x++)
        for(i32 z = 0; z < CHUNKS_Z; z++) {
            i += 1;
            int cutoff = width * i / max;
            int percent = 100 * i / max;

            std::cout << "\r[";

            for(int j = 0; j < width; j++)
                if (j == cutoff)
                    std::cout << ">";
                else if (j < cutoff)
                    std::cout << "=";
                else
                    std::cout << " ";

            std::cout << "] - " << percent << "%" << std::flush;

            GenerateChunk(V2(x, z));
        }

    auto end = std::chrono::system_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::printf("\nChunk gen took %ld ms\n", ms);
}

void World::Draw() {
    for (auto [v, model] : models) {
        V3 pos(
            (f32)(v.x * CHUNK_WIDTH - (i32)(CHUNKS_X * CHUNK_WIDTH / 2)),
            0,
            (f32)(v.y * CHUNK_WIDTH - (i32)(CHUNKS_Z * CHUNK_WIDTH / 2))
        );
        DrawModel(models[v], pos, 1, WHITE);
    }
}

