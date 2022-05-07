#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glm/glm.hpp>

#define GLEW_STATIC
#include <GL/glew.h>

#include <map>
#include <string>
#include <vector>

class FallingSandEngine;

class TextRenderer final {
public:
    static TextRenderer& Instance() {
        static TextRenderer t;
        return t;
    }

    struct Character {
        unsigned int texture, nextOffset;
        glm::ivec2 size, offset;
    };

    std::map<char, Character> characters;

    TextRenderer();
    ~TextRenderer();
    void RenderText(FallingSandEngine* app,
        std::string text,
        GLfloat x, GLfloat y,
        const glm::vec2& scale = glm::vec2(1.f, 1.f),
        const glm::vec4& color = glm::vec4(1.f, 1.f, 1.f, 1.f));

    TextRenderer(TextRenderer const&) = delete;
    void operator=(TextRenderer const&) = delete;

private:
    unsigned int shader, vao, vbo, tris;
    FT_Library ft;
    FT_Face face;
};