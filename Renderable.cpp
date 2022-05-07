#include "Renderable.hpp"
#include "FallingSandEngine.hpp"
#include "Shaders.hpp"

#include <glm/gtc/type_ptr.hpp>

#include <iostream>

Renderable::Renderable(FallingSandEngine* app, unsigned int verts, unsigned int colors, unsigned int uvs) {
    this->app = app;
    this->verts = verts;
    this->colors = colors;
    this->uvs = uvs;
    shader = Shaders::defaultShaderProgram;

    glGenVertexArrays(1, &vertexArrayId);
    glGenBuffers(1, &vertexBufferId);
    glGenBuffers(1, &elementBufferId);
    glGenTextures(1, &textureId);
    // placeholder white texture
    unsigned char tmpWhite[4] = { 255, 255, 255, 255 };
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D,
        0,
        GL_RGBA,
        1,
        1,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        tmpWhite);
}

Renderable::~Renderable() {
    glDeleteTextures(1, &textureId);
    glDeleteBuffers(1, &vertexBufferId);
    glDeleteBuffers(1, &elementBufferId);
    glDeleteVertexArrays(1, &vertexArrayId);
}

void Renderable::Render() {
    if (!initialized) {
        InitializeGPUData();
    }

    glUseProgram(shader);
    glBindVertexArray(vertexArrayId);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    if (verts < 3) {
        glUniformMatrix4fv(glGetUniformLocation(shader, "proj"), 1, GL_FALSE, glm::value_ptr(app->GetUICamera()->GetProjection()));
    } else {
        glUniformMatrix4fv(glGetUniformLocation(shader, "proj"), 1, GL_FALSE, glm::value_ptr(app->GetWorldCamera()->GetProjection()));
        glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, glm::value_ptr(app->GetWorldCamera()->GetTransform()));
    }
    glUniformMatrix4fv(glGetUniformLocation(shader, "transform"), 1, GL_FALSE, glm::value_ptr(transform));
    glUniform4fv(glGetUniformLocation(shader, "colorMod"), 1, glm::value_ptr(colorMod));

    if (elements.empty()) {
        return;
    }
    glDrawElements(GL_TRIANGLES, elements.size(), GL_UNSIGNED_INT, (void*)0);
}

void Renderable::InitializeGPUData() {
    if (vertices.empty()) {
        return;
    }
    glBindVertexArray(vertexArrayId);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);

    glUseProgram(shader);
    int sizeTotal = verts + uvs + colors;

    glVertexAttribPointer(0, verts, GL_FLOAT, GL_FALSE, sizeTotal * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, uvs, GL_FLOAT, GL_FALSE, sizeTotal * sizeof(GLfloat), (GLvoid*)(verts * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, colors, GL_FLOAT, GL_FALSE, sizeTotal * sizeof(GLfloat), (GLvoid*)((verts + uvs) * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements.size() * sizeof(GLuint), &elements[0], GL_STATIC_DRAW);

    initialized = true;
}