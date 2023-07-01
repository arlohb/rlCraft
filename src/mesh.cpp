#include "mesh.h"

#include <utility>
#include <vector>
#include <cstring>

#include "types.h"

struct Face {
    V3 pos;
    Dir dir;
    V2 texCoords;
};

i32 NaturalTexturesOffset(Face face) {
    i32 x = (i32)face.pos.x;
    i32 y = (i32)face.pos.y;
    i32 z = (i32)face.pos.z;

    u32 seed = 0;
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

void SetVertex(Mesh *mesh, size_t i, Face face, V3 offset) {
    V3 pos = face.pos + offset;

    mesh->vertices[i * 3    ] = pos.x;
    mesh->vertices[i * 3 + 1] = pos.y;
    mesh->vertices[i * 3 + 2] = pos.z;

    V3 normal = DirVector(face.dir);

    mesh->normals[i * 3    ] = normal.x;
    mesh->normals[i * 3 + 1] = normal.y;
    mesh->normals[i * 3 + 2] = normal.z;

    V2 texOffset;

    i32 naturalOffset = NaturalTexturesOffset(face);

    switch ((i + naturalOffset) % 4) {
        case 0:
            texOffset = V2(0, 1);
            break;
        case 1:
            texOffset = V2(0, 0);
            break;
        case 2:
            texOffset = V2(1, 0);
            break;
        case 3:
            texOffset = V2(1, 1);
            break;
    }

    texOffset /= 16.0;

    V2 texCoord = face.texCoords + texOffset;

    mesh->texcoords[i * 2    ] = texCoord.x;
    mesh->texcoords[i * 2 + 1] = texCoord.y;
}

void SetTri(Mesh *mesh, size_t i, u16 a, u16 b, u16 c) {
    mesh->indices[i * 3    ] = a;
    mesh->indices[i * 3 + 1] = b;
    mesh->indices[i * 3 + 2] = c;
}

void SetTriWithVertexOffset(Mesh *mesh, usize i, usize offset, u16 a, u16 b, u16 c) {
    SetTri(mesh, i, a + offset, b + offset, c + offset);
}

void SetFace(Mesh *mesh, usize i, Face face) {
    usize verticesPerFace = 4;
    usize vertexOffset = i * verticesPerFace;
    usize trisPerFace = 2;
    usize triOffset = i * trisPerFace;

    // Center the mesh at y=0
    face.pos.y -= CHUNK_HEIGHT / 2.0;
    
    V3 o1, o2, o3, o4;

    switch (face.dir) {
        using enum Dir;
        case Px:
            o1 = V3(1, 0, 1);
            o2 = V3(1, 1, 1);
            o3 = V3(1, 1, 0);
            o4 = V3(1, 0, 0);
            break;
        case Nx:
            o1 = V3(0, 0, 0);
            o2 = V3(0, 1, 0);
            o3 = V3(0, 1, 1);
            o4 = V3(0, 0, 1);
            break;
        case Py:
            o1 = V3(0, 1, 0);
            o2 = V3(1, 1, 0);
            o3 = V3(1, 1, 1);
            o4 = V3(0, 1, 1);
            break;
        case Ny:
            o1 = V3(0, 0, 0);
            o2 = V3(0, 0, 1);
            o3 = V3(1, 0, 1);
            o4 = V3(1, 0, 0);
            break;
        case Pz:
            o1 = V3(0, 0, 1);
            o2 = V3(0, 1, 1);
            o3 = V3(1, 1, 1);
            o4 = V3(1, 0, 1);
            break;
        case Nz:
            o1 = V3(1, 0, 0);
            o2 = V3(1, 1, 0);
            o3 = V3(0, 1, 0);
            o4 = V3(0, 0, 0);
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

void MaybeAddFace(std::vector<Face>& faces, V3 pos, V3 negOffset, Dir dir, Block b1, Block b2) {
    Block blockFace = GetBlockFace(b1, b2);

    if (IsBlock(b1) && !IsBlock(b2))
        faces.push_back({
            .pos = pos,
            .dir = dir,
            .texCoords = BlockTexCoords(blockFace),
        });

    if (!IsBlock(b1) && IsBlock(b2))
        faces.push_back({
            .pos = Vector3Add(pos, negOffset),
            .dir = (Dir)((i32)dir + 1),
            .texCoords = BlockTexCoords(blockFace),
        });
}

Model CreateModel(Chunk& chunk) {
    std::vector<Face> faces;

    for(i32 x = 0; x < CHUNK_WIDTH; x++)
        for(i32 z = 0; z < CHUNK_WIDTH; z++)
            for(i32 y = 0; y < CHUNK_HEIGHT; y++) {
                Block block = chunk.GetBlock(x, y, z);

                V3 pos(x, y, z);

                if (x == 0) MaybeAddFace(faces, pos, V3(-1, 0, 0), Dir::Nx, block, Block::Air);
                if (y == 0) MaybeAddFace(faces, pos, V3(0, -1, 0), Dir::Ny, block, Block::Air);
                if (z == 0) MaybeAddFace(faces, pos, V3(0, 0, -1), Dir::Nz, block, Block::Air);

                Block px = (x != CHUNK_WIDTH - 1) ? chunk.GetBlock(x + 1, y, z) : Block::Air;
                MaybeAddFace(faces, pos, V3(1, 0, 0), Dir::Px, block, px);

                Block py = (y != CHUNK_HEIGHT - 1) ? chunk.GetBlock(x, y + 1, z) : Block::Air;
                MaybeAddFace(faces, pos, V3(0, 1, 0), Dir::Py, block, py);

                Block pz = (z != CHUNK_WIDTH - 1) ? chunk.GetBlock(x, y, z + 1) : Block::Air;
                MaybeAddFace(faces, pos, V3(0, 0, 1), Dir::Pz, block, pz);
            }

    // This is fine on the stack,
    // as Mesh only contains i32s and poi32ers,
    // so UploadMesh doesn't need mesh to stay alive,
    // and LoadModelFromMesh takes by value.
    Mesh mesh = {};

    mesh.triangleCount = faces.size() * 2;
    mesh.vertexCount = faces.size() * 4;
    mesh.vertices = new f32[mesh.vertexCount * 3];
    mesh.normals = new f32[mesh.vertexCount * 3];
    mesh.indices = new u16[mesh.triangleCount * 3];
    mesh.texcoords = new f32[mesh.vertexCount * 2];

    i32 i = 0;
    for(Face face : faces) {
        SetFace(&mesh, i, face);

        i++;
    }

    UploadMesh(&mesh, false);

    return LoadModelFromMesh(mesh);
}

