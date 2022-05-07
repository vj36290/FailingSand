#pragma once

#include <glm/glm.hpp>
#define GLEW_STATIC
#include "GL/glew.h"

#include <vector>

class FallingSandEngine;

class Renderable {
public:
    friend class FallingSandEngine;
    Renderable(FallingSandEngine* app, unsigned int verts, unsigned int colors, unsigned int uvs);
    virtual ~Renderable();
    virtual void Render();
protected:
    FallingSandEngine* app;
    std::vector<GLfloat> vertices;
    std::vector<GLuint> elements;
    glm::mat4 transform = glm::mat4(1.f);
    glm::vec4 colorMod = glm::vec4(1.f);
    GLuint vertexBufferId, elementBufferId, vertexArrayId, textureId, shader;

private:
    bool initialized = false;
    unsigned int verts, colors, uvs;

private:
    void InitializeGPUData();
};