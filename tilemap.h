#pragma once
#include "Chunk.h"

class TileMap {
public:
    int widthInChunks;
    int heightInChunks;

    Chunk** chunks;

    TileMap(int w, int h) {
        widthInChunks = w;
        heightInChunks = h;

        int totalChunks = widthInChunks * heightInChunks;

        chunks = new Chunk*[totalChunks];

        for (int y = 0; y < heightInChunks; ++y) {
            for (int x = 0; x < widthInChunks; ++x) {
                chunks[y * widthInChunks + x] = new Chunk(x, y);
            }
        }
    }

    ~TileMap() {
        int totalChunks = widthInChunks * heightInChunks;
        for (int i = 0; i < totalChunks; ++i) {
            delete chunks[i];
        }
        delete[] chunks;
    }

    int getTile(int worldX, int worldY) {
        if (worldX < 0 || worldY < 0) return -1;

        int chunkX = worldX / CHUNK_SIZE;
        int chunkY = worldY / CHUNK_SIZE;

        if (chunkX >= widthInChunks || chunkY >= heightInChunks) return -1;

        int localX = worldX % CHUNK_SIZE;
        int localY = worldY % CHUNK_SIZE;

        return chunks[chunkY * widthInChunks + chunkX]->getTile(localX, localY);
    }

    void setTile(int worldX, int worldY, int tileID) {
        if (worldX < 0 || worldY < 0) return;

        int chunkX = worldX / CHUNK_SIZE;
        int chunkY = worldY / CHUNK_SIZE;

        if (chunkX >= widthInChunks || chunkY >= heightInChunks) return;

        int localX = worldX % CHUNK_SIZE;
        int localY = worldY % CHUNK_SIZE;

        chunks[chunkY * widthInChunks + chunkX]->setTile(localX, localY, tileID);
    }

    void render() {
        int totalChunks = widthInChunks * heightInChunks;
        for (int i = 0; i < totalChunks; ++i) {
            chunks[i]->render();
        }
    }
};
