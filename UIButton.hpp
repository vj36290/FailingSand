#pragma once

#include "UIBox.hpp"

#include <functional>

class UIButton : public UIBox {
    UIButton(FallingSandEngine* app, std::string name,
        UIElement* parent = nullptr, 
        const glm::vec2& position = glm::vec2(0.f),
        const glm::vec2& dimensions = glm::vec2(0.f),
        const glm::vec2& anchor = glm::vec2(0.f),
        const glm::vec2& dock = glm::vec2(0.f),
        float lineWidth = 1.f,
        const glm::vec4& lineColor = glm::vec4(1.f, 1.f, 1.f, 1.f),
        const glm::vec4& fillColor = glm::vec4(0.f, 0.f, 0.f, 1.f));

    //void Execute();
};