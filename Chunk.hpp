#pragma once
#define CHUNK_SIZE 32
#define CHUNK_CELLS CHUNK_SIZE * CHUNK_SIZE

#define GLEW_STATIC
#include <GL/glew.h>

#include "Renderable.hpp"
#include "Cell.hpp"

class World;
class FallingSandEngine;

class Chunk final : public Renderable {
public:
    const int chunkX, chunkY;

public:
    Chunk(FallingSandEngine* app, World* world, int chunkX, int chunkY);
    ~Chunk();
    void Reset(bool randomSeed = false);
    Cell* At(unsigned int x, unsigned int y);
    void Render();
    void Update(double deltaTime);

private:
    World* world;
    std::vector<Cell*> cells;
    unsigned char chunkTexture[CHUNK_CELLS * 4];
};