#pragma once

const int CHUNK_SIZE = 16;

class Chunk {
public:
    int chunkX;
    int chunkY;
    int* tiles;

    Chunk(int x, int y) {
        chunkX = x;
        chunkY = y;

        tiles = new int[CHUNK_SIZE * CHUNK_SIZE];

        for (int i = 0; i < CHUNK_SIZE * CHUNK_SIZE; ++i) {
            tiles[i] = 0;
        }
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
            tiles[localY * CHUNK_SIZE + localX] = tileID;
        }
    }
};
