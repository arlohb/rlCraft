#include "world.h"

#include "mesh.h"
#include <iostream>

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

void World::GenerateChunk(rl::Vector2 v) {
    Chunk chunk;
    chunk.pos = v;
    chunk.Generate();
    chunks.insert(std::pair<rl::Vector2, Chunk>(v, chunk));

    Model model = CreateModel(chunk);
    model.materials[0] = worldMat;
    models.insert(std::pair<rl::Vector2, Model>(v, model));
}

void World::Draw() {
    for (auto [v, model] : models) {
        rl::Vector3 pos(
            (float)(v.x * CHUNK_WIDTH - (int)(CHUNKS_X * CHUNK_WIDTH / 2)),
            0,
            (float)(v.y * CHUNK_WIDTH - (int)(CHUNKS_Z * CHUNK_WIDTH / 2))
        );
        DrawModel(models[v], pos, 1, WHITE);
    }
}

