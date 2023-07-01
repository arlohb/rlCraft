#include "block.h"
#include "chunk.h"

#include <vector>
#include <cstring>
#include <raylib.h>
#include <raymath.h>

struct Face {
    rl::Vector3 pos;
    Dir dir;
    rl::Vector2 texCoords;
};

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

void SetVertex(Mesh *mesh, size_t i, Face face, rl::Vector3 offset) {
    rl::Vector3 pos = face.pos + offset;

    mesh->vertices[i * 3    ] = pos.x;
    mesh->vertices[i * 3 + 1] = pos.y;
    mesh->vertices[i * 3 + 2] = pos.z;

    rl::Vector3 normal = DirVector(face.dir);

    mesh->normals[i * 3    ] = normal.x;
    mesh->normals[i * 3 + 1] = normal.y;
    mesh->normals[i * 3 + 2] = normal.z;

    rl::Vector2 texOffset;

    int naturalOffset = NaturalTexturesOffset(face);

    switch ((i + naturalOffset) % 4) {
        case 0:
            texOffset = rl::Vector2(0, 1);
            break;
        case 1:
            texOffset = rl::Vector2(0, 0);
            break;
        case 2:
            texOffset = rl::Vector2(1, 0);
            break;
        case 3:
            texOffset = rl::Vector2(1, 1);
            break;
    }

    texOffset /= 16.0;

    rl::Vector2 texCoord = face.texCoords + texOffset;

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

    // Center the mesh at y=0
    face.pos.y -= CHUNK_HEIGHT / 2.0;
    
    rl::Vector3 o1, o2, o3, o4;

    switch (face.dir) {
        using enum Dir;
        case Px:
            o1 = rl::Vector3(1, 0, 1);
            o2 = rl::Vector3(1, 1, 1);
            o3 = rl::Vector3(1, 1, 0);
            o4 = rl::Vector3(1, 0, 0);
            break;
        case Nx:
            o1 = rl::Vector3(0, 0, 0);
            o2 = rl::Vector3(0, 1, 0);
            o3 = rl::Vector3(0, 1, 1);
            o4 = rl::Vector3(0, 0, 1);
            break;
        case Py:
            o1 = rl::Vector3(0, 1, 0);
            o2 = rl::Vector3(1, 1, 0);
            o3 = rl::Vector3(1, 1, 1);
            o4 = rl::Vector3(0, 1, 1);
            break;
        case Ny:
            o1 = rl::Vector3(0, 0, 0);
            o2 = rl::Vector3(0, 0, 1);
            o3 = rl::Vector3(1, 0, 1);
            o4 = rl::Vector3(1, 0, 0);
            break;
        case Pz:
            o1 = rl::Vector3(0, 0, 1);
            o2 = rl::Vector3(0, 1, 1);
            o3 = rl::Vector3(1, 1, 1);
            o4 = rl::Vector3(1, 0, 1);
            break;
        case Nz:
            o1 = rl::Vector3(1, 0, 0);
            o2 = rl::Vector3(1, 1, 0);
            o3 = rl::Vector3(0, 1, 0);
            o4 = rl::Vector3(0, 0, 0);
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

void MaybeAddFace(std::vector<Face>& faces, rl::Vector3 pos, rl::Vector3 negOffset, Dir dir, Block b1, Block b2) {
    Block blockFace = GetBlockFace(b1, b2);

    if (IsBlock(b1) && !IsBlock(b2)) {
        Face face;
        face.pos = pos;
        face.dir = dir;
        face.texCoords = BlockTexCoords(blockFace);
        faces.push_back(face);
    }

    if (!IsBlock(b1) && IsBlock(b2)) {
        Face face;
        face.pos = Vector3Add(pos, negOffset);
        face.dir = (Dir)((int)dir + 1);
        face.texCoords = BlockTexCoords(blockFace);
        faces.push_back(face);
    }
}

Model CreateModel(Chunk& chunk) {
    std::vector<Face> faces;

    for(int x = 0; x < CHUNK_WIDTH; x++)
        for(int z = 0; z < CHUNK_WIDTH; z++)
            for(int y = 0; y < CHUNK_HEIGHT; y++) {
                Block block = chunk.GetBlock(x, y, z);

                rl::Vector3 pos(x, y, z);

                if (x == 0) MaybeAddFace(faces, pos, rl::Vector3(-1, 0, 0), Dir::Nx, block, Block::Air);
                if (y == 0) MaybeAddFace(faces, pos, rl::Vector3(0, -1, 0), Dir::Ny, block, Block::Air);
                if (z == 0) MaybeAddFace(faces, pos, rl::Vector3(0, 0, -1), Dir::Nz, block, Block::Air);

                Block px = (x != CHUNK_WIDTH - 1) ? chunk.GetBlock(x + 1, y, z) : Block::Air;
                MaybeAddFace(faces, pos, rl::Vector3(1, 0, 0), Dir::Px, block, px);

                Block py = (y != CHUNK_HEIGHT - 1) ? chunk.GetBlock(x, y + 1, z) : Block::Air;
                MaybeAddFace(faces, pos, rl::Vector3(0, 1, 0), Dir::Py, block, py);

                Block pz = (z != CHUNK_WIDTH - 1) ? chunk.GetBlock(x, y, z + 1) : Block::Air;
                MaybeAddFace(faces, pos, rl::Vector3(0, 0, 1), Dir::Pz, block, pz);
            }

    // This is fine on the stack,
    // as Mesh only contains ints and pointers,
    // so UploadMesh doesn't need mesh to stay alive,
    // and LoadModelFromMesh takes by value.
    Mesh mesh = {};

    mesh.triangleCount = faces.size() * 2;
    mesh.vertexCount = faces.size() * 4;
    mesh.vertices = new float[mesh.vertexCount * 3];
    mesh.normals = new float[mesh.vertexCount * 3];
    mesh.indices = new unsigned short[mesh.triangleCount * 3];
    mesh.texcoords = new float[mesh.vertexCount * 2];

    int i = 0;
    for(Face face : faces) {
        SetFace(&mesh, i, face);

        i++;
    }

    UploadMesh(&mesh, false);

    return LoadModelFromMesh(mesh);
}

