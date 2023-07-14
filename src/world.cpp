#include "world.h"

#include <chrono>
#include "mesh.h"
#include "progressBar.h"

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
    chunks.insert(std::make_pair(v, chunk));

    Model model = CreateModel(chunk);
    model.materials[0] = worldMat;
    models.insert(std::make_pair(v, model));
}

void World::InitialGeneration() {
    auto start = std::chrono::system_clock::now();

    i32 i = 0;
    const i32 radius = 8;
    const int max = pow(radius * 2 + 1, 2);

    for(i32 x = -radius; x <= radius; x++)
        for(i32 z = -radius; z <= radius; z++) {
            DrawBar((f32)++i / max, 80);

            GenerateChunk(V2(x, z));
        }

    auto end = std::chrono::system_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::printf("\nChunk gen took %ld ms\n", ms);
}

void World::Draw() {
    for (auto [v, model] : models) {
        V3 pos(
            v.x * CHUNK_WIDTH,
            0,
            v.y * CHUNK_WIDTH
        );
        DrawModel(models[v], pos, 1, WHITE);
    }
}

