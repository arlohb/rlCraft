#pragma once

#include <unordered_map>

#include "chunk.h"

class World {
    public:
        World();
        ~World();

        void GenerateChunk(V2);
        void InitialGeneration();
        void Draw();

    private:
        rl::Texture worldTex;
        // Can't use class as *I think* Material unloads the shader itself.
        Shader worldShader;
        rl::Material worldMat;

        Noise noise;

        std::unordered_map<V2, Chunk> chunks;
        std::unordered_map<V2, Model> models;
};

