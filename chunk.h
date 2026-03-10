#pragma once

const int CHUNK_SIZE = 16;
const float TILE_SIZE = 32.0f;

class Chunk {
public:
    int chunkX;
    int chunkY;
    int* tiles;

    float* vertices;
    int vertexCount;

    Chunk(int x, int y) {
        chunkX = x;
        chunkY = y;

        tiles = new int[CHUNK_SIZE * CHUNK_SIZE];

        vertices = new float[CHUNK_SIZE * CHUNK_SIZE * 4 * 2];
        vertexCount = 0;

        for (int i = 0; i < CHUNK_SIZE * CHUNK_SIZE; ++i) {
            tiles[i] = 1;
        }

        generateMesh();
    }

    ~Chunk() {
        delete[] tiles;
        delete[] vertices;
    }

    int getTile(int localX, int localY) {
        if (localX >= 0 && localX < CHUNK_SIZE && localY >= 0 && localY < CHUNK_SIZE) {
            return tiles[localY * CHUNK_SIZE + localX];
        }
        return -1;
    }

    void setTile(int localX, int localY, int tileID) {
        if (localX >= 0 && localX < CHUNK_SIZE && localY >= 0 && localY < CHUNK_SIZE) {
            tiles[localY * CHUNK_SIZE + localX] = tileID;

            generateMesh();
        }
    }

    void generateMesh() {
        vertexCount = 0;
        int vertexIndex = 0;

        for (int y = 0; y < CHUNK_SIZE; ++y) {
            for (int x = 0; x < CHUNK_SIZE; ++x) {
                int tileID = getTile(x, y);

                if (tileID > 0) {
                    float startX = (chunkX * CHUNK_SIZE + x) * TILE_SIZE;
                    float startY = (chunkY * CHUNK_SIZE + y) * TILE_SIZE;

                    vertices[vertexIndex++] = startX;
                    vertices[vertexIndex++] = startY;

                    vertices[vertexIndex++] = startX + TILE_SIZE;
                    vertices[vertexIndex++] = startY;

                    vertices[vertexIndex++] = startX + TILE_SIZE;
                    vertices[vertexIndex++] = startY + TILE_SIZE;
                    vertices[vertexIndex++] = startX;
                    vertices[vertexIndex++] = startY + TILE_SIZE;

                    vertexCount += 4;
                }
            }
        }
    }
};
