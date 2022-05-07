#include "TextRenderer.hpp"
#include "Shaders.hpp"
#include "FallingSandEngine.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>

/*************************************************************************************************
    * Constructor largely from https://learnopengl.com/In-Practice/Text-Rendering.
**************************************************************************************************/

using namespace glm;

TextRenderer::TextRenderer() {
    ft = FT_Library();
    if (FT_Init_FreeType(&ft)) {
        std::cout << "failure" << std::endl;
        return;
    }

    face = FT_Face();
    if (FT_New_Face(ft, "fonts/vcr.ttf", 0, &face)) {
        std::cout << "Failed to load font." << std::endl;
        return;
    }

    FT_Set_Pixel_Sizes(face, 0, 12);
    //glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (unsigned char c = 0; c < 128; c++)
    {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "Failed to load Freetype glyph for character '" << c << "'." << std::endl;
            continue;
        }

        GLuint textureId;
        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character character = {
            textureId, face->glyph->advance.x,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
        };
        characters.insert(std::pair<char, Character>(c, character));
    }

    GLfloat vertices[] = {
        0.f, 0.f,    0.f, 0.f,       1.f, 1.f, 1.f, 1.f,
        1.f, 0.f,    1.f, 0.f,       1.f, 1.f, 1.f, 1.f,
        0.f, 1.f,    0.f, 1.f,       1.f, 1.f, 1.f, 1.f,
        1.f, 1.f,    1.f, 1.f,       1.f, 1.f, 1.f, 1.f,
    };

    GLuint elements[] = {
        0, 2, 1,
        2, 3, 1
    };

    shader = Shaders::defaultUIShaderProgram;

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &tris);

    glUseProgram(shader);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 4 * 8 * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(4 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tris);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), &elements[0], GL_STATIC_DRAW);
}

TextRenderer::~TextRenderer() {
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

void TextRenderer::RenderText(FallingSandEngine* app,
    std::string text,
    GLfloat x, GLfloat y,
    const glm::vec2& scale,
    const glm::vec4& color) {

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tris);

    for (auto c = text.begin(); c != text.end(); c++)
    {
        Character ch = characters[*c];
        float xPos = x + ch.offset.x;
        float yPos = y - (ch.size.y - ch.offset.y);
        mat4 transform =
            glm::scale(
                glm::translate(mat4(1.f), vec3(xPos, yPos, 0.f)),
                vec3(scale, scale.x)
            );
        glUniformMatrix4fv(glGetUniformLocation(shader, "transform"), 1, GL_FALSE, glm::value_ptr(transform));

        glBindTexture(GL_TEXTURE_2D, ch.texture);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

        x += (ch.nextOffset >> 6);
    }
}