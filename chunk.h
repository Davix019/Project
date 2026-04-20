#pragma once
#include "sfw.h"

const int CHUNK_SIZE = 16;
const float TILE_SIZE = 32.0f;

class Chunk {
public:
    int chunkX;
    int chunkY;
    int* tiles;

    Ref<Mesh> _mesh;
    Ref<ColorMaterial2D> _mat;

    Chunk(int x, int y) {
        chunkX = x;
        chunkY = y;

        tiles = new int[CHUNK_SIZE * CHUNK_SIZE];

        for (int i = 0; i < CHUNK_SIZE * CHUNK_SIZE; ++i) {
            tiles[i] = 1;
        }

        _mesh.instance();
        _mat.instance();

        generateMesh();
    }

    ~Chunk() {
        delete[] tiles;
    }

    int getTile(int localX, int localY) {
        if (localX >= 0 && localX < CHUNK_SIZE && localY >= 0 && localY < CHUNK_SIZE) {
            return tiles[localY * CHUNK_SIZE + localX];
        }
        return -1;
    }

    void setTile(int localX, int localY, int tileID) {
        if (localX >= 0 && localX < CHUNK_SIZE && localY >= 0 && localY < CHUNK_SIZE) {
            if (tiles[localY * CHUNK_SIZE + localX] != tileID) {
                tiles[localY * CHUNK_SIZE + localX] = tileID;
                generateMesh();
            }
        }
    }

    void generateMesh() {
        _mesh->clear();
        _mesh->vertex_dimesions = 2;

        uint32_t vertexIndex = 0;

        for (int y = 0; y < CHUNK_SIZE; ++y) {
            for (int x = 0; x < CHUNK_SIZE; ++x) {
                int tileID = getTile(x, y);

                if (tileID > 0) {
                    float startX = (chunkX * CHUNK_SIZE + x) * TILE_SIZE;
                    float startY = (chunkY * CHUNK_SIZE + y) * TILE_SIZE;

                    Color tileColor;
                    if (tileID == 1) tileColor = Color(0.0f, 0.6f, 0.0f);
                    else if (tileID == 2) tileColor = Color(0.5f, 0.5f, 0.5f);
                    else if (tileID == 3) tileColor = Color(1.0f, 0.8f, 0.0f);
                    else if (tileID == 4) tileColor = Color(0.6f, 0.0f, 0.8f);
                    else tileColor = Color(0.0f, 0.0f, 0.0f);

                    _mesh->add_color(tileColor);
                    _mesh->add_vertex2(startX, startY);

                    _mesh->add_color(tileColor);
                    _mesh->add_vertex2(startX + TILE_SIZE, startY);

                    _mesh->add_color(tileColor);
                    _mesh->add_vertex2(startX + TILE_SIZE, startY + TILE_SIZE);

                    _mesh->add_color(tileColor);
                    _mesh->add_vertex2(startX, startY + TILE_SIZE);

                    _mesh->add_triangle(vertexIndex + 0, vertexIndex + 2, vertexIndex + 1);
                    _mesh->add_triangle(vertexIndex + 0, vertexIndex + 3, vertexIndex + 2);

                    vertexIndex += 4;
                }
            }
        }
        _mesh->upload();
    }

    void render() {
        if (_mesh.is_valid()) {
            _mat->bind();
            _mesh->render();
        }
    }
};
