#include "World.hpp"
#include "Chunk.hpp"
#include "Cell.hpp"
#include "Material.hpp"

#include <iostream>

World::World(FallingSandEngine* app) {
    this->app = app;
    this->chunks = SignedIndexArray<SignedIndexArray<Chunk*>>(MAX_ACTIVE_CHUNKS + 1);

    for (int i = MIN_CHUNK_INDEX; i <= MAX_CHUNK_INDEX; i++) {
        chunks[i] = SignedIndexArray<Chunk*>(MAX_ACTIVE_CHUNKS + 1);
        for (int k = MIN_CHUNK_INDEX; k <= MAX_CHUNK_INDEX; k++)
            chunks[i][k] = new Chunk(app, this, i, k);
    }
}

World::~World() {
    for (int i = MIN_CHUNK_INDEX; i <= MAX_CHUNK_INDEX; i++) {
        for (int k = MIN_CHUNK_INDEX; k <= MAX_CHUNK_INDEX; k++)
            delete chunks[i][k];
    }
}

glm::ivec4 World::ToChunkCellCoords(int worldX, int worldY) {
    int maxCellX = CHUNK_SIZE - 1;
    int maxCellY = CHUNK_SIZE - 1;

    int chunkX, chunkY, cellX, cellY;

    if (worldX >= 0) {
        chunkX = worldX / CHUNK_SIZE;
        cellX = (worldX % CHUNK_SIZE);
    }
    else if (worldX < 0) {
        chunkX = ((int)(worldX + 0.5f) / CHUNK_SIZE) - 1;
        cellX = maxCellX + (worldX % CHUNK_SIZE) + 1;
        if (cellX == CHUNK_SIZE) {
            cellX = 0;
        }
    }

    if (worldY >= 0) {
        chunkY = worldY / CHUNK_SIZE;
        cellY = worldY % CHUNK_SIZE;
    }
    else if (worldY < 0) {
        chunkY = ((int) (worldY + 0.5f) / CHUNK_SIZE) - 1;
        cellY = maxCellY + (worldY % CHUNK_SIZE) + 1;
        if (cellY == CHUNK_SIZE) {
            cellY = 0;
        }
    }

    return glm::ivec4(chunkX, chunkY, cellX, cellY);
}

Cell* World::GetCell(int worldX, int worldY) {
    glm::ivec4 coords = ToChunkCellCoords(worldX, worldY);
    
    if (coords[0] < MIN_CHUNK_INDEX || coords[0] > MAX_CHUNK_INDEX
        || coords[1] < MIN_CHUNK_INDEX || coords[1] > MAX_CHUNK_INDEX)
        return nullptr;

    return chunks[coords[0]][coords[1]]->At(coords[2], coords[3]);
}

void World::Reset(bool randomSeed) {
    for (int i = MIN_CHUNK_INDEX; i <= MAX_CHUNK_INDEX; i++) {
        for (int k = MIN_CHUNK_INDEX; k <= MAX_CHUNK_INDEX; k++)
            (chunks[i][k])->Reset(randomSeed);
    }
}

Chunk* World::GetChunk(int chunkX, int chunkY) {
    if (chunkX < MIN_CHUNK_INDEX || chunkX > MAX_CHUNK_INDEX
        || chunkY < MIN_CHUNK_INDEX || chunkY > MAX_CHUNK_INDEX)
        return nullptr;
    return chunks[chunkX][chunkY];
}

void World::Render() {
    for (int i = MIN_CHUNK_INDEX; i <= MAX_CHUNK_INDEX; i++) {
        for (int k = MIN_CHUNK_INDEX; k <= MAX_CHUNK_INDEX; k++)
            (chunks[i][k])->Render();
    }
}

void World::Update(double deltaTime) {
    for (int i = MIN_CHUNK_INDEX; i <= MAX_CHUNK_INDEX; i++) {
        for (int k = MIN_CHUNK_INDEX; k <= MAX_CHUNK_INDEX; k++)
            chunks[i][k]->Update(deltaTime);
    }
}