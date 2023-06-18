#include <stdio.h>
#include <raylib.h>
#include <raymath.h>

#include "3dText.h"

#define WIDTH 800
#define HEIGHT 600

typedef enum {
    PX,
    NX,
    PY,
    NY,
    PZ,
    NZ,
} Direction;

void SetVertex(Mesh* mesh, size_t i, Vector3 position) {
    mesh->vertices[i * 3    ] = position.x;
    mesh->vertices[i * 3 + 1] = position.y;
    mesh->vertices[i * 3 + 2] = position.z;

    Vector3 normal = Vector3Normalize(Vector3SubtractValue(position, 0.5));

    mesh->normals[i * 3    ] = normal.x;
    mesh->normals[i * 3 + 1] = normal.y;
    mesh->normals[i * 3 + 2] = normal.z;
}

void SetTri(Mesh* mesh, size_t i, unsigned short a, unsigned short b, unsigned short c) {
    mesh->indices[i * 3    ] = a;
    mesh->indices[i * 3 + 1] = b;
    mesh->indices[i * 3 + 2] = c;
}

void SetTriWithVertexOffset(Mesh* mesh, size_t i, size_t offset, unsigned short a, unsigned short b, unsigned short c) {
    SetTri(mesh, i,
        a + offset,
        b + offset,
        c + offset
    );
}

void SetFace(Mesh* mesh, size_t i, Vector3 pos, Direction dir) {
    size_t verticesPerFace = 4;
    size_t vertexOffset = i * verticesPerFace;
    size_t trisPerFace = 2;
    size_t triOffset = i * trisPerFace;

    switch (dir) {
        case PX:
            SetVertex(mesh, vertexOffset    , Vector3Add(pos, (Vector3){ 1, 0, 1 }));
            SetVertex(mesh, vertexOffset + 1, Vector3Add(pos, (Vector3){ 1, 1, 1 }));
            SetVertex(mesh, vertexOffset + 2, Vector3Add(pos, (Vector3){ 1, 1, 0 }));
            SetVertex(mesh, vertexOffset + 3, Vector3Add(pos, (Vector3){ 1, 0, 0 }));
            break;
        case NX:
            SetVertex(mesh, vertexOffset    , Vector3Add(pos, (Vector3){ 0, 0, 0 }));
            SetVertex(mesh, vertexOffset + 1, Vector3Add(pos, (Vector3){ 0, 1, 0 }));
            SetVertex(mesh, vertexOffset + 2, Vector3Add(pos, (Vector3){ 0, 1, 1 }));
            SetVertex(mesh, vertexOffset + 3, Vector3Add(pos, (Vector3){ 0, 0, 1 }));
            break;
        case PY:
            SetVertex(mesh, vertexOffset    , Vector3Add(pos, (Vector3){ 0, 1, 0 }));
            SetVertex(mesh, vertexOffset + 1, Vector3Add(pos, (Vector3){ 1, 1, 0 }));
            SetVertex(mesh, vertexOffset + 2, Vector3Add(pos, (Vector3){ 1, 1, 1 }));
            SetVertex(mesh, vertexOffset + 3, Vector3Add(pos, (Vector3){ 0, 1, 1 }));
            break;
        case NY:
            SetVertex(mesh, vertexOffset    , Vector3Add(pos, (Vector3){ 0, 0, 0 }));
            SetVertex(mesh, vertexOffset + 1, Vector3Add(pos, (Vector3){ 0, 0, 1 }));
            SetVertex(mesh, vertexOffset + 2, Vector3Add(pos, (Vector3){ 1, 0, 1 }));
            SetVertex(mesh, vertexOffset + 3, Vector3Add(pos, (Vector3){ 1, 0, 0 }));
            break;
        case PZ:
            SetVertex(mesh, vertexOffset    , Vector3Add(pos, (Vector3){ 0, 0, 1 }));
            SetVertex(mesh, vertexOffset + 1, Vector3Add(pos, (Vector3){ 0, 1, 1 }));
            SetVertex(mesh, vertexOffset + 2, Vector3Add(pos, (Vector3){ 1, 1, 1 }));
            SetVertex(mesh, vertexOffset + 3, Vector3Add(pos, (Vector3){ 1, 0, 1 }));
            break;
        case NZ:
            SetVertex(mesh, vertexOffset    , Vector3Add(pos, (Vector3){ 1, 0, 0 }));
            SetVertex(mesh, vertexOffset + 1, Vector3Add(pos, (Vector3){ 1, 1, 0 }));
            SetVertex(mesh, vertexOffset + 2, Vector3Add(pos, (Vector3){ 0, 1, 0 }));
            SetVertex(mesh, vertexOffset + 3, Vector3Add(pos, (Vector3){ 0, 0, 0 }));
            break;
        default:
            break;
    }

    // Winding order seems to be anti-clockwise
    SetTriWithVertexOffset(mesh, triOffset    , vertexOffset, 2, 1, 0);
    SetTriWithVertexOffset(mesh, triOffset + 1, vertexOffset, 3, 2, 0);
}

void AddCube(Mesh* mesh, size_t i, Vector3 pos) {
    SetFace(mesh, i * 6    , pos, PX);
    SetFace(mesh, i * 6 + 1, pos, NX);
    SetFace(mesh, i * 6 + 2, pos, PY);
    SetFace(mesh, i * 6 + 3, pos, NY);
    SetFace(mesh, i * 6 + 4, pos, PZ);
    SetFace(mesh, i * 6 + 5, pos, NZ);
}

int main() {
    InitWindow(WIDTH, HEIGHT, "RLCraft");

    Mesh mesh = {};

    int cubeCount = 3;
    mesh.triangleCount = cubeCount * 12;
    mesh.vertexCount = cubeCount * 24;
    mesh.vertices = (float*)MemAlloc(mesh.vertexCount * 3 * sizeof(float));
    // mesh.texcoords = (float*)MemAlloc(mesh.vertexCount * 2 * sizeof(float));
    mesh.normals = (float*)MemAlloc(mesh.vertexCount * 3 * sizeof(float));
    mesh.indices = (unsigned short*)MemAlloc(mesh.triangleCount * 3 * sizeof(unsigned short));

    AddCube(&mesh, 0, (Vector3){ 0, 0, 0 });
    AddCube(&mesh, 1, (Vector3){ 3, 0, 0 });
    AddCube(&mesh, 2, (Vector3){ 0, 0, 3 });

    UploadMesh(&mesh, false);

    Model model = LoadModelFromMesh(mesh);

    Camera3D camera = {};
    camera.position = (Vector3) { 0, 10, 10 };
    camera.target = Vector3Zero();
    camera.up = (Vector3) { 0, 1, 0 };
    camera.fovy = 45;
    camera.projection = CAMERA_PERSPECTIVE;
    SetCameraMode(camera, CAMERA_FREE);

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(camera);

                // Draw the grid, axis, and axis labels
                DrawGrid(10, 1);
                DrawLine3D(Vector3Zero(), (Vector3){ 10, 0, 0 }, RED);
                DrawText3D(GetFontDefault(), "x", (Vector3){ 10, 0, 0 }, 15, 0, 0, true, RED);
                DrawLine3D(Vector3Zero(), (Vector3){ 0, 10, 0 }, GREEN);
                DrawText3D(GetFontDefault(), "y", (Vector3){ 0, 10, 0 }, 15, 0, 0, true, GREEN);
                DrawLine3D(Vector3Zero(), (Vector3){ 0, 0, 10 }, BLUE);
                DrawText3D(GetFontDefault(), "z", (Vector3){ 0, 0, 10 }, 15, 0, 0, true, BLUE);

                // Draw the model
                DrawModel(model, Vector3Zero(), 1, GREEN);

                // Draw the model vertices
                for(int i = 0; i < mesh.vertexCount; i++) {
                    Vector3 pos = {
                        model.meshes[0].vertices[i * 3],
                        model.meshes[0].vertices[i * 3 + 1],
                        model.meshes[0].vertices[i * 3 + 2],
                    };

                    DrawSphere(pos, 0.1, RED);
                }

                UpdateCamera(&camera);

            EndMode3D();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
