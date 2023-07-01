#pragma once

#include <unordered_map>
#include <memory>

#include "chunk.h"
#include "rl.h"

#define CHUNKS_X 16
#define CHUNKS_Z 16

class World {
    public:
        World();
        ~World();

        void GenerateChunk(rl::Vector2);
        void Draw();

    private:
        rl::Texture worldTex;
        // Can't use class as *I think* Material unloads the shader itself.
        Shader worldShader;
        rl::Material worldMat;

        std::unordered_map<rl::Vector2, Chunk> chunks;
        std::unordered_map<rl::Vector2, Model> models;
};

