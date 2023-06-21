#include "linkedList.h"
#include "block.h"
#include "chunk.h"
#include "mesh.h"

#include <stddef.h>
#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <stdlib.h>

void SetVertex(Mesh *mesh, size_t i, Vector3 position, Direction dir, Color colour) {
    mesh->vertices[i * 3] = position.x;
    mesh->vertices[i * 3 + 1] = position.y;
    mesh->vertices[i * 3 + 2] = position.z;

    Vector3 normal = DirectionVector(dir);

    mesh->normals[i * 3] = normal.x;
    mesh->normals[i * 3 + 1] = normal.y;
    mesh->normals[i * 3 + 2] = normal.z;

    mesh->colors[i * 4] = colour.r;
    mesh->colors[i * 4 + 1] = colour.g;
    mesh->colors[i * 4 + 2] = colour.b;
    mesh->colors[i * 4 + 3] = colour.a;
}

void SetTri(Mesh *mesh, size_t i, unsigned short a, unsigned short b, unsigned short c) {
    mesh->indices[i * 3] = a;
    mesh->indices[i * 3 + 1] = b;
    mesh->indices[i * 3 + 2] = c;
}

void SetTriWithVertexOffset(Mesh *mesh, size_t i, size_t offset,
                            unsigned short a, unsigned short b, unsigned short c) {
    SetTri(mesh, i, a + offset, b + offset, c + offset);
}

void SetFace(Mesh *mesh, size_t i, Vector3 pos, Direction dir, Color colour) {
    size_t verticesPerFace = 4;
    size_t vertexOffset = i * verticesPerFace;
    size_t trisPerFace = 2;
    size_t triOffset = i * trisPerFace;

    pos.y -= 127;

    switch (dir) {
    case PX:
        SetVertex(mesh, vertexOffset    , Vector3Add(pos, (Vector3){1, 0, 1}), dir, colour);
        SetVertex(mesh, vertexOffset + 1, Vector3Add(pos, (Vector3){1, 1, 1}), dir, colour);
        SetVertex(mesh, vertexOffset + 2, Vector3Add(pos, (Vector3){1, 1, 0}), dir, colour);
        SetVertex(mesh, vertexOffset + 3, Vector3Add(pos, (Vector3){1, 0, 0}), dir, colour);
        break;
    case NX:
        SetVertex(mesh, vertexOffset    , Vector3Add(pos, (Vector3){0, 0, 0}), dir, colour);
        SetVertex(mesh, vertexOffset + 1, Vector3Add(pos, (Vector3){0, 1, 0}), dir, colour);
        SetVertex(mesh, vertexOffset + 2, Vector3Add(pos, (Vector3){0, 1, 1}), dir, colour);
        SetVertex(mesh, vertexOffset + 3, Vector3Add(pos, (Vector3){0, 0, 1}), dir, colour);
        break;
    case PY:
        SetVertex(mesh, vertexOffset    , Vector3Add(pos, (Vector3){0, 1, 0}), dir, colour);
        SetVertex(mesh, vertexOffset + 1, Vector3Add(pos, (Vector3){1, 1, 0}), dir, colour);
        SetVertex(mesh, vertexOffset + 2, Vector3Add(pos, (Vector3){1, 1, 1}), dir, colour);
        SetVertex(mesh, vertexOffset + 3, Vector3Add(pos, (Vector3){0, 1, 1}), dir, colour);
        break;
    case NY:
        SetVertex(mesh, vertexOffset    , Vector3Add(pos, (Vector3){0, 0, 0}), dir, colour);
        SetVertex(mesh, vertexOffset + 1, Vector3Add(pos, (Vector3){0, 0, 1}), dir, colour);
        SetVertex(mesh, vertexOffset + 2, Vector3Add(pos, (Vector3){1, 0, 1}), dir, colour);
        SetVertex(mesh, vertexOffset + 3, Vector3Add(pos, (Vector3){1, 0, 0}), dir, colour);
        break;
    case PZ:
        SetVertex(mesh, vertexOffset    , Vector3Add(pos, (Vector3){0, 0, 1}), dir, colour);
        SetVertex(mesh, vertexOffset + 1, Vector3Add(pos, (Vector3){0, 1, 1}), dir, colour);
        SetVertex(mesh, vertexOffset + 2, Vector3Add(pos, (Vector3){1, 1, 1}), dir, colour);
        SetVertex(mesh, vertexOffset + 3, Vector3Add(pos, (Vector3){1, 0, 1}), dir, colour);
        break;
    case NZ:
        SetVertex(mesh, vertexOffset    , Vector3Add(pos, (Vector3){1, 0, 0}), dir, colour);
        SetVertex(mesh, vertexOffset + 1, Vector3Add(pos, (Vector3){1, 1, 0}), dir, colour);
        SetVertex(mesh, vertexOffset + 2, Vector3Add(pos, (Vector3){0, 1, 0}), dir, colour);
        SetVertex(mesh, vertexOffset + 3, Vector3Add(pos, (Vector3){0, 0, 0}), dir, colour);
        break;
    default:
        break;
    }

    // Winding order seems to be anti-clockwise
    SetTriWithVertexOffset(mesh, triOffset, vertexOffset, 2, 1, 0);
    SetTriWithVertexOffset(mesh, triOffset + 1, vertexOffset, 3, 2, 0);
}

Model CreateModel(Chunk* chunk) {
    List faces;
    ListInit(&faces);

    for(int x = 0; x < CHUNK_WIDTH - 1; x++)
        for(int z = 0; z < CHUNK_WIDTH - 1; z++)
            for(int y = 0; y < CHUNK_HEIGHT - 1; y++) {
                Block block = ChunkGetBlock(chunk, x, y, z);

                Block px = ChunkGetBlock(chunk, x + 1, y, z);
                Block py = ChunkGetBlock(chunk, x, y + 1, z);
                Block pz = ChunkGetBlock(chunk, x, y, z + 1);

                Block blockFace;

                if ((blockFace = GetBlockFace(block, px))) {
                    Face* face = malloc(sizeof(Face));
                    face->pos = (Vector3){ x, y, z };
                    face->dir = PX;
                    face->colour = BlockColour(blockFace);
                    ListAppend(&faces, face);
                }

                if ((blockFace = GetBlockFace(block, py))) {
                    Face* face = malloc(sizeof(Face));
                    face->pos = (Vector3){ x, y, z };
                    face->dir = PY;
                    face->colour = BlockColour(blockFace);
                    ListAppend(&faces, face);
                }

                if ((blockFace = GetBlockFace(block, pz))) {
                    Face* face = malloc(sizeof(Face));
                    face->pos = (Vector3){ x, y, z };
                    face->dir = PZ;
                    face->colour = BlockColour(blockFace);
                    ListAppend(&faces, face);
                }
            }

    // This is fine on the stack,
    // as Mesh only contains ints and pointers,
    // so UploadMesh doesn't need mesh to stay alive,
    // and LoadModelFromMesh takes by value.
    Mesh mesh = {};

    mesh.triangleCount = faces.length * 2;
    mesh.vertexCount = faces.length * 4;
    mesh.vertices = malloc(mesh.vertexCount * 3 * sizeof(float));
    mesh.normals = malloc(mesh.vertexCount * 3 * sizeof(float));
    mesh.indices = malloc(mesh.triangleCount * 3 * sizeof(unsigned short));
    mesh.colors = malloc(mesh.vertexCount * 4 * sizeof(unsigned char));

    int i = 0;
    ListIter(faces, node) {
      Face *face = node->val;

      SetFace(&mesh, i, face->pos, face->dir, face->colour);

      // I'm deleting the values,
      // but not the nodes yet,
      // because idk how
      free(face);

      i++;
    }

    UploadMesh(&mesh, false);

    return LoadModelFromMesh(mesh);
}

