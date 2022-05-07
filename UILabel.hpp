#pragma once

#include "UIElement.hpp"

class UILabel : public UIElement {
public:
    UILabel(FallingSandEngine* app, const std::string& name, const std::string& text,
        UIElement* parent = nullptr,
        const glm::vec4& color = glm::vec4(1.f, 1.f, 1.f, 1.f),
        const glm::vec2& position = glm::vec2(0.f),
        const glm::vec2& dimensions = glm::vec2(0.f),
        const glm::vec2& anchor = glm::vec2(0.f),
        const glm::vec2& dock = glm::vec2(0.f));

    virtual void Render();
private:
    std::string text;
};