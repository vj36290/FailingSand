#include "UIButton.hpp"

using namespace glm;

UIButton::UIButton(FallingSandEngine* app, std::string name, 
    UIElement* parent, 
    const vec2& position, const vec2& dimensions,
    const vec2& anchor, const vec2& dock,
    float lineWidth, const vec4& lineColor, const vec4& fillColor) :
    UIBox(app, name, parent, position, dimensions, anchor, dock,
        lineWidth, lineColor, fillColor) {

}