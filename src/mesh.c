#include "linkedList.h"
#include "block.h"
#include "chunk.h"
#include "mesh.h"

#include <stddef.h>
#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    Vector3 pos;
    Direction dir;
    Vector2 texCoords;
} Face;

int NaturalTexturesOffset(Face face) {
    int x = (int)face.pos.x;
    int y = (int)face.pos.y;
    int z = (int)face.pos.z;

    unsigned int seed = 0;
    memcpy(&seed, &x, 1);
    seed <<= 8;
    memcpy(&seed, &y, 1);
    seed <<= 8;
    memcpy(&seed, &z, 1);
    seed <<= 8;
    memcpy(&seed, &face.dir, 1);

    SetRandomSeed(seed);

    return GetRandomValue(0, 3);
}

void SetVertex(Mesh *mesh, size_t i, Face face, Vector3 offset) {
    Vector3 pos = Vector3Add(face.pos, offset);

    mesh->vertices[i * 3    ] = pos.x;
    mesh->vertices[i * 3 + 1] = pos.y;
    mesh->vertices[i * 3 + 2] = pos.z;

    Vector3 normal = DirectionVector(face.dir);

    mesh->normals[i * 3    ] = normal.x;
    mesh->normals[i * 3 + 1] = normal.y;
    mesh->normals[i * 3 + 2] = normal.z;

    Vector2 texOffset;

    int naturalOffset = NaturalTexturesOffset(face);

    switch ((i + naturalOffset) % 4) {
        case 0:
            texOffset = (Vector2){ 0, 1 };
            break;
        case 1:
            texOffset = (Vector2){ 0, 0 };
            break;
        case 2:
            texOffset = (Vector2){ 1, 0 };
            break;
        case 3:
            texOffset = (Vector2){ 1, 1 };
            break;
    }

    texOffset = Vector2Scale(texOffset, 1.0 / 16.0);

    Vector2 texCoord = Vector2Add(face.texCoords, texOffset);

    mesh->texcoords[i * 2    ] = texCoord.x;
    mesh->texcoords[i * 2 + 1] = texCoord.y;
}

void SetTri(Mesh *mesh, size_t i, unsigned short a, unsigned short b, unsigned short c) {
    mesh->indices[i * 3    ] = a;
    mesh->indices[i * 3 + 1] = b;
    mesh->indices[i * 3 + 2] = c;
}

void SetTriWithVertexOffset(Mesh *mesh, size_t i, size_t offset,
                            unsigned short a, unsigned short b, unsigned short c) {
    SetTri(mesh, i, a + offset, b + offset, c + offset);
}

void SetFace(Mesh *mesh, size_t i, Face face) {
    size_t verticesPerFace = 4;
    size_t vertexOffset = i * verticesPerFace;
    size_t trisPerFace = 2;
    size_t triOffset = i * trisPerFace;

    face.pos.y -= 127;
    
    Vector3 o1, o2, o3, o4;

    switch (face.dir) {
    case PX:
        o1 = (Vector3){1, 0, 1};
        o2 = (Vector3){1, 1, 1};
        o3 = (Vector3){1, 1, 0};
        o4 = (Vector3){1, 0, 0};
        break;
    case NX:
        o1 = (Vector3){0, 0, 0};
        o2 = (Vector3){0, 1, 0};
        o3 = (Vector3){0, 1, 1};
        o4 = (Vector3){0, 0, 1};
        break;
    case PY:
        o1 = (Vector3){0, 1, 0};
        o2 = (Vector3){1, 1, 0};
        o3 = (Vector3){1, 1, 1};
        o4 = (Vector3){0, 1, 1};
        break;
    case NY:
        o1 = (Vector3){0, 0, 0};
        o2 = (Vector3){0, 0, 1};
        o3 = (Vector3){1, 0, 1};
        o4 = (Vector3){1, 0, 0};
        break;
    case PZ:
        o1 = (Vector3){0, 0, 1};
        o2 = (Vector3){0, 1, 1};
        o3 = (Vector3){1, 1, 1};
        o4 = (Vector3){1, 0, 1};
        break;
    case NZ:
        o1 = (Vector3){1, 0, 0};
        o2 = (Vector3){1, 1, 0};
        o3 = (Vector3){0, 1, 0};
        o4 = (Vector3){0, 0, 0};
        break;
    default:
        break;
    }

    SetVertex(mesh, vertexOffset    , face, o1);
    SetVertex(mesh, vertexOffset + 1, face, o2);
    SetVertex(mesh, vertexOffset + 2, face, o3);
    SetVertex(mesh, vertexOffset + 3, face, o4);

    // Winding order seems to be anti-clockwise
    SetTriWithVertexOffset(mesh, triOffset    , vertexOffset, 2, 1, 0);
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
                    face->texCoords = BlockTexCoords(blockFace);
                    ListAppend(&faces, face);
                }

                if ((blockFace = GetBlockFace(block, py))) {
                    Face* face = malloc(sizeof(Face));
                    face->pos = (Vector3){ x, y, z };
                    face->dir = PY;
                    face->texCoords = BlockTexCoords(blockFace);
                    ListAppend(&faces, face);
                }

                if ((blockFace = GetBlockFace(block, pz))) {
                    Face* face = malloc(sizeof(Face));
                    face->pos = (Vector3){ x, y, z };
                    face->dir = PZ;
                    face->texCoords = BlockTexCoords(blockFace);
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
    mesh.texcoords = malloc(mesh.vertexCount * 2 * sizeof(float));

    int i = 0;
    ListIter(faces, node) {
      Face *face = node->val;

      SetFace(&mesh, i, *face);

      // I'm deleting the values,
      // but not the nodes yet,
      // because idk how
      free(face);

      i++;
    }

    UploadMesh(&mesh, false);

    return LoadModelFromMesh(mesh);
}

