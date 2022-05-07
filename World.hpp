#pragma once
#include <glm/glm.hpp>

#include "SignedIndexArray.hpp"

#ifndef MAX_ACTIVE_CHUNKS
#define MAX_ACTIVE_CHUNKS 8
#endif

#ifndef MIN_CHUNK_INDEX
#define MIN_CHUNK_INDEX MAX_ACTIVE_CHUNKS / -2
#endif

#ifndef MAX_CHUNK_INDEX
#define MAX_CHUNK_INDEX MAX_ACTIVE_CHUNKS / 2 - 1
#endif

class Chunk;
class Cell;
class FallingSandEngine;

class World final {
public:
    friend class FallingSandEngine;
    World(FallingSandEngine* app);
    ~World();

    glm::ivec4 ToChunkCellCoords(int worldX, int worldY);
    Chunk* GetChunk(int chunkX, int chunkY);
    Cell* GetCell(int worldX, int worldY);

    void Reset(bool randomSeed = false);
private:
    FallingSandEngine* app;
    SignedIndexArray<SignedIndexArray<Chunk*>> chunks;
private:
    void Render();
    void Update(double deltaTime);
};