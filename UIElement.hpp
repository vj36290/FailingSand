#pragma once

#include "Renderable.hpp"
#include <vector>
#include <string>

class Camera;
class FallingSandEngine;

class UIElement : public Renderable {
public:
    glm::vec3 position = glm::vec3(0.f);
public:
    UIElement(FallingSandEngine* app, const std::string& name,
        UIElement* parent = nullptr,
        const glm::vec2& position = glm::vec2(0.f),
        const glm::vec2& dimensions = glm::vec2(0.f),
        const glm::vec2& anchor = glm::vec2(0.f),
        const glm::vec2& dock = glm::vec2(0.f));
    virtual ~UIElement();

    const std::string& Name() const;

    const glm::vec2& Dimensions() const;
    const glm::vec2& Anchor() const;
    const glm::vec2& Dock() const;
    const glm::mat4& Bounds() const;
    const bool Contains(const glm::vec2& point) const;

    const std::vector<UIElement*>& GetChildren() const;
    const int Depth() const;

    virtual bool OnClick(const glm::vec2& clickedPoint);

    virtual void Render();
protected:
    std::string name;
    glm::vec2 offset = glm::vec2(0.f);
    glm::vec2 anchor = glm::vec2(0.f);
    glm::vec2 dock = glm::vec2(0.f);
    glm::vec2 dimensions = glm::vec2(0.f);
    glm::mat4 bounds;
    UIElement* parent;
    std::vector<UIElement*> children;
    Camera* projectionCamera;
    int depth;
};