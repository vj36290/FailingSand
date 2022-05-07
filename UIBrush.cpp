#include "UIBrush.hpp"
#include "FallingSandEngine.hpp"

#include "glm/gtc/matrix_transform.hpp"

using namespace glm;

UIBrush::UIBrush(FallingSandEngine* app) :
    Renderable(app, 3, 4, 2) {

    float lineWidth = 0.1f;
    vec2 tL = vec2(0.f, 0.f);
    vec2 tR = vec2(1.f - lineWidth, 0.f);
    vec2 bL = vec2(0.f, 1.f - lineWidth);
    vec2 bR = vec2(1.f - lineWidth, 1.f - lineWidth);
    vec4 lineColor = vec4(1.f, 1.f, 1.f, 1.f);

    vertices = {
        // Top left corner of border
        tL.x,               tL.y,               0.f,    0.f, 1.f,   lineColor.x, lineColor.y, lineColor.z, lineColor.w, // 0
        tL.x + lineWidth,   tL.y,               0.f,    0.f, 1.f,   lineColor.x, lineColor.y, lineColor.z, lineColor.w, // 1
        tL.x,               tL.y + lineWidth,   0.f,    0.f, 1.f,   lineColor.x, lineColor.y, lineColor.z, lineColor.w, // 2
        tL.x + lineWidth,   tL.y + lineWidth,   0.f,    0.f, 1.f,   lineColor.x, lineColor.y, lineColor.z, lineColor.w, // 3

        // Top right corner of border
        tR.x,               tR.y,               0.f,    0.f, 1.f,   lineColor.x, lineColor.y, lineColor.z, lineColor.w, // 4
        tR.x + lineWidth,   tR.y,               0.f,    0.f, 1.f,   lineColor.x, lineColor.y, lineColor.z, lineColor.w, // 5
        tR.x,               tR.y + lineWidth,   0.f,    0.f, 1.f,   lineColor.x, lineColor.y, lineColor.z, lineColor.w, // 6
        tR.x + lineWidth,   tR.y + lineWidth,   0.f,    0.f, 1.f,   lineColor.x, lineColor.y, lineColor.z, lineColor.w, // 7

        // Bottom left corner of border
        bL.x,               bL.y,               0.f,    0.f, 1.f,   lineColor.x, lineColor.y, lineColor.z, lineColor.w, // 8
        bL.x + lineWidth,   bL.y,               0.f,    0.f, 1.f,   lineColor.x, lineColor.y, lineColor.z, lineColor.w, // 9
        bL.x,               bL.y + lineWidth,   0.f,    0.f, 1.f,   lineColor.x, lineColor.y, lineColor.z, lineColor.w, // 10
        bL.x + lineWidth,   bL.y + lineWidth,   0.f,    0.f, 1.f,   lineColor.x, lineColor.y, lineColor.z, lineColor.w, // 11

        // Bottom right corner of border
        bR.x,               bR.y,               0.f,    0.f, 1.f,   lineColor.x, lineColor.y, lineColor.z, lineColor.w, // 12
        bR.x + lineWidth,   bR.y,               0.f,    0.f, 1.f,   lineColor.x, lineColor.y, lineColor.z, lineColor.w, // 13
        bR.x,               bR.y + lineWidth,   0.f,    0.f, 1.f,   lineColor.x, lineColor.y, lineColor.z, lineColor.w, // 14
        bR.x + lineWidth,   bR.y + lineWidth,   0.f,    0.f, 1.f,   lineColor.x, lineColor.y, lineColor.z, lineColor.w, // 15
    };

    elements = {
        // Top left
        0, 2, 1,
        2, 3, 1,

        // Top right
        4, 6, 5,
        6, 7, 5,

        // Bottom left
        8, 10, 9,
        10, 11, 9,

        // Bottom right
        12, 14, 13,
        14, 15, 13,

        // Top
        1, 3, 4,
        3, 6, 4,

        // Left
        2, 8, 3,
        8, 9, 3,

        // Right
        6, 12, 7,
        12, 13, 7,

        // Bottom
        9, 11, 12,
        11, 14, 12
    };
}

void UIBrush::Render() {
    vec3 translate = app->WindowToWorldPoint(app->GetMousePosition());
    vec3 scale = vec3(app->GetBrushSize(), app->GetBrushSize(), 0.f);
    for (int i = 0; i < 3; i++) {
        if (translate[i] < 0)
            translate[i] = ceilf(translate[i]);
        else
            translate[i] = floorf(translate[i]);
    }

    transform = glm::scale(glm::translate(mat4(1.f), translate), scale);
    this->Renderable::Render();
}