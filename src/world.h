#pragma once

#include <unordered_map>
#include <memory>

#include "chunk.h"

const i32 CHUNKS_X = 16;
const i32 CHUNKS_Z = 16;

class World {
    public:
        World();
        ~World();

        void GenerateChunk(V2);
        void Draw();

    private:
        rl::Texture worldTex;
        // Can't use class as *I think* Material unloads the shader itself.
        Shader worldShader;
        rl::Material worldMat;

        std::unordered_map<V2, Chunk> chunks;
        std::unordered_map<V2, Model> models;
};

