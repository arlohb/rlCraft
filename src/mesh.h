#ifndef _MESH_H
#define _MESH_H

#include "linkedList.h"
#include "block.h"
#include "chunk.h"

#include <raymath.h>

typedef struct {
    Vector3 pos;
    Direction dir;
    Color colour;
} Face;

void SetVertex(Mesh *mesh, size_t i, Vector3 position, Direction dir, Color colour);

void SetTri(Mesh *mesh, size_t i, unsigned short a, unsigned short b, unsigned short c);

void SetTriWithVertexOffset(Mesh *mesh, size_t i, size_t offset,
        unsigned short a, unsigned short b, unsigned short c);

void SetFace(Mesh *mesh, size_t i, Vector3 pos, Direction dir, Color colour);

Model CreateModel(Chunk* chunk);

#endif
