#include "world.h"

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
    chunk.Generate();
    chunks.insert(std::pair<V2, Chunk>(v, chunk));

    Model model = CreateModel(chunk);
    model.materials[0] = worldMat;
    models.insert(std::pair<V2, Model>(v, model));
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

