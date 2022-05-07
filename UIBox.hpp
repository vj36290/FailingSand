#pragma once
#include "UIElement.hpp"

#include <glm/glm.hpp>

class UIBox : public UIElement {
public:
    float lineWidth = 1.f;
    glm::vec4 lineColor, fillColor;
public:
    UIBox(FallingSandEngine* app, std::string name,
        UIElement* parent = nullptr,
        const glm::vec2& position = glm::vec2(0.f),
        const glm::vec2& dimensions = glm::vec2(0.f),
        const glm::vec2& anchor = glm::vec2(0.f),
        const glm::vec2& dock = glm::vec2(0.f),
        float lineWidth = 1.f,
        const glm::vec4& lineColor = glm::vec4(1.f, 1.f, 1.f, 1.f),
        const glm::vec4& fillColor = glm::vec4(0.f, 0.f, 0.f, 1.f));
    virtual ~UIBox();
    virtual void Render();
};