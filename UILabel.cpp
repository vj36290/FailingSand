#include "UILabel.hpp"

#include "TextRenderer.hpp"

#include <iostream>

using namespace glm;

UILabel::UILabel(FallingSandEngine* app, const std::string& name, const std::string& text,
    UIElement* parent,
    const vec4& color,
    const vec2& position, const vec2& dimensions,
    const vec2& anchor, const vec2& dock) :
    UIElement(app, name, parent, position, dimensions, anchor, dock) {

    this->text = text;
}

void UILabel::Render() {
    this->UIElement::Render();
    TextRenderer::Instance().RenderText(app, text, position.x, position.y, vec2(100.f, 100.f));
}