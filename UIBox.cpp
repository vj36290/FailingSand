#include "UIBox.hpp"

using namespace glm;

UIBox::UIBox(FallingSandEngine* app, std::string name, UIElement* parent,
    const vec2& position, const vec2& dimensions, 
    const vec2& anchor, const vec2& dock,
    float lineWidth, const vec4& lineColor, const vec4& fillColor) :
    UIElement(app, name, parent, position, dimensions, anchor, dock), 
    lineWidth(lineWidth), lineColor(lineColor), fillColor(fillColor) {

    // draw border as a series of thin boxes, basically
    // these mark the top left corners of each corner box
    vec2 tL = vec2(0.f);
    vec2 tR = vec2(dimensions.x - lineWidth, 0.f);
    vec2 bL = vec2(0.f, dimensions.y - lineWidth);
    vec2 bR = vec2(dimensions.x - lineWidth, dimensions.y - lineWidth);

    vertices = {
        // Top left corner of border
        tL.x, tL.y,                         0.f, 1.f,   lineColor.x, lineColor.y, lineColor.z, lineColor.w, // 0
        tL.x + lineWidth, tL.y,             0.f, 1.f,   lineColor.x, lineColor.y, lineColor.z, lineColor.w, // 1
        tL.x, tL.y + lineWidth,             0.f, 1.f,   lineColor.x, lineColor.y, lineColor.z, lineColor.w, // 2
        tL.x + lineWidth, tL.y + lineWidth, 0.f, 1.f,   lineColor.x, lineColor.y, lineColor.z, lineColor.w, // 3

        // Top right corner of border
        tR.x, tR.y,                         0.f, 1.f,   lineColor.x, lineColor.y, lineColor.z, lineColor.w, // 4
        tR.x + lineWidth, tR.y,             0.f, 1.f,   lineColor.x, lineColor.y, lineColor.z, lineColor.w, // 5
        tR.x, tR.y + lineWidth,             0.f, 1.f,   lineColor.x, lineColor.y, lineColor.z, lineColor.w, // 6
        tR.x + lineWidth, tR.y + lineWidth, 0.f, 1.f,   lineColor.x, lineColor.y, lineColor.z, lineColor.w, // 7

        // Bottom left corner of border
        bL.x, bL.y,                         0.f, 1.f,   lineColor.x, lineColor.y, lineColor.z, lineColor.w, // 8
        bL.x + lineWidth, bL.y,             0.f, 1.f,   lineColor.x, lineColor.y, lineColor.z, lineColor.w, // 9
        bL.x, bL.y + lineWidth,             0.f, 1.f,   lineColor.x, lineColor.y, lineColor.z, lineColor.w, // 10
        bL.x + lineWidth, bL.y + lineWidth, 0.f, 1.f,   lineColor.x, lineColor.y, lineColor.z, lineColor.w, // 11

        // Bottom right corner of border
        bR.x, bR.y,                         0.f, 1.f,   lineColor.x, lineColor.y, lineColor.z, lineColor.w, // 12
        bR.x + lineWidth, bR.y,             0.f, 1.f,   lineColor.x, lineColor.y, lineColor.z, lineColor.w, // 13
        bR.x, bR.y + lineWidth,             0.f, 1.f,   lineColor.x, lineColor.y, lineColor.z, lineColor.w, // 14
        bR.x + lineWidth, bR.y + lineWidth, 0.f, 1.f,   lineColor.x, lineColor.y, lineColor.z, lineColor.w, // 15

        // fill
        tL.x + lineWidth, tL.y + lineWidth, 0.f, 1.f,   fillColor.x, fillColor.y, fillColor.z, fillColor.w, // 16
        tR.x, tR.y + lineWidth,             0.f, 1.f,   fillColor.x, fillColor.y, fillColor.z, fillColor.w, // 17
        bL.x + lineWidth, bL.y,             0.f, 1.f,   fillColor.x, fillColor.y, fillColor.z, fillColor.w, // 18
        bR.x, bR.y,                         0.f, 1.f,   fillColor.x, fillColor.y, fillColor.z, fillColor.w, // 19
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
        11, 14, 12,

        // Fill
        16, 18, 17,
        18, 19, 17
    };
}

UIBox::~UIBox() {

}

void UIBox::Render() {
    this->UIElement::Render();
}