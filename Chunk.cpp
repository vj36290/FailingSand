#include "Chunk.hpp"
#include "World.hpp"
#include "Material.hpp"
#include "Shaders.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <iostream>

#include <random>

using namespace glm;

Chunk::Chunk(FallingSandEngine* app, World* world, int chunkX, int chunkY)
    : Renderable(app, 3, 4, 2), world(world), chunkX(chunkX), chunkY(chunkY) {
    cells = std::vector<Cell*>(CHUNK_CELLS);
    shader = Shaders::defaultShaderProgram;
    Reset();

    vertices = {
        0.f, 0.f, 0.f,  0.f, 0.f,       1.f, 1.f, 1.f, 1.f,
        1.f, 0.f, 0.f,  1.f, 0.f,       1.f, 1.f, 1.f, 1.f,
        0.f, 1.f, 0.f,  0.f, 1.f,       1.f, 1.f, 1.f, 1.f,
        1.f, 1.f, 0.f,  1.f, 1.f,       1.f, 1.f, 1.f, 1.f,
    };

    elements = {
        0, 2, 1,
        2, 3, 1
    };

    transform =
        scale(
            translate(mat4(1.f), vec3(chunkX * CHUNK_SIZE, chunkY * CHUNK_SIZE, 0.f)),
            vec3(CHUNK_SIZE, CHUNK_SIZE, 1.f)
        );
}

Chunk::~Chunk() {
    for (int y = 0; y < CHUNK_SIZE; y++) {
        for (int x = 0; x < CHUNK_SIZE; x++) {
            size_t index = y * CHUNK_SIZE + x;
            if (cells[index])
                delete cells[index];
        }
    }
}

void Chunk::Reset(bool randomSeed) {
    int materials = Materials::Instance().NumMaterials();
    for (int y = 0; y < CHUNK_SIZE; y++) {
        for (int x = 0; x < CHUNK_SIZE; x++) {
            size_t index = y * CHUNK_SIZE + x;
            unsigned int materialId = randomSeed ? rand() % Materials::Instance().NumMaterials() : 0;
            if (!cells[index]) {
                int cellX = CHUNK_SIZE * chunkX + x;
                int cellY = CHUNK_SIZE * chunkY + y;
                cells[index] = new Cell(world, this,
                    cellX, cellY,
                    materialId);
            }
            else {
                cells[index]->material = Materials::Instance().Get(materialId);
            }
        }
    }
}

Cell* Chunk::At(unsigned int x, unsigned int y) {
    size_t index = (size_t)y * (size_t)CHUNK_SIZE + (size_t)x;
    return cells[index];
}

void Chunk::Render() {
    for (size_t y = 0; y < CHUNK_SIZE; y++) {
        for (size_t x = 0; x < CHUNK_SIZE; x++) {
            const unsigned char* data = At(x, y)->material->texture.data();
            for (int k = 0; k < 4; k++)
                chunkTexture[(y * CHUNK_SIZE + x) * 4 + k] = data[k];
        }
    }

    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D,
        0,
        GL_RGBA,
        CHUNK_SIZE,
        CHUNK_SIZE,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        chunkTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    this->Renderable::Render();
}

void Chunk::Update(double deltaTime) {
    float currentTime = glfwGetTime();

    for (int y = CHUNK_SIZE - 1; y >= 0; y--) {
        for (int x = CHUNK_SIZE - 1; x >= 0; x--) {
            At(x, y)->Update(deltaTime);
        }
    }
}