#include "Camera.hpp"
#include "UIElement.hpp"
#include "Shaders.hpp"
#include "FallingSandEngine.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

using namespace glm;

UIElement::UIElement(FallingSandEngine* app, const std::string& name, UIElement* parent,
    const glm::vec2& position, const glm::vec2& dimensions,
    const glm::vec2& anchor, const glm::vec2& dock) :
    Renderable(app, 2, 4, 2) {

    shader = Shaders::defaultUIShaderProgram;
    projectionCamera = app->GetUICamera();

    this->parent = parent;
    if (!parent)
        depth = 0;
    else {
        depth = parent->depth + 1;
        parent->children.push_back(this);
    }
    this->name = name;
    this->offset = position;
    this->dimensions = dimensions;
    this->anchor = anchor;
    this->dock = dock;

    // dock should refer to where in the window the element is positioned
    // anchor should refer to the location in the element that will be positioned
    // at the dock point, so (0, 0) is top left and (1, 1) is bottom right
    vec2 anchorPoint = anchor * dimensions;
    vec2 dockPosition = dock * vec2(app->windowWidth, app->windowHeight);

    vec2 topLeft = app->WindowToScreenPoint(dockPosition);
    vec2 topRight = topLeft + vec2(dimensions.x, 0.f);
    vec2 bottomLeft = topLeft + vec2(0.f, dimensions.y);
    vec2 bottomRight = topLeft + dimensions;

    bounds = mat4(vec4(topLeft, 0.f, 1.f),
        vec4(topRight, 0.f, 1.f),
        vec4(bottomLeft, 0.f, 1.f),
        vec4(bottomRight, 0.f, 1.f));
    // since screen space operates in a +/- coordinate system, we want to
    // translate those coordinates
    this->position = vec3(bounds[0]) + vec3(position, 0.f);
    transform = translate(mat4(1.f), this->position);
    children = std::vector<UIElement*>();
}

UIElement::~UIElement() {
    for (auto it = children.begin(); it != children.end(); ++it)
        delete* it;
}

const vec2& UIElement::Anchor() const {
    return anchor;
}

const std::string& UIElement::Name() const {
    return name;
}

const vec2& UIElement::Dimensions() const {
    return dimensions;
}

const vec2& UIElement::Dock() const {
    return dock;
}

const mat4& UIElement::Bounds() const {
    return bounds;
}

const bool UIElement::Contains(const vec2& point) const {
    vec2 tl = vec2(bounds[0]);
    vec2 br = vec2(bounds[3]);
    return point.x >= tl.x && point.x <= br.x &&
        point.y >= tl.y && point.y <= br.y;
}

const std::vector<UIElement*>& UIElement::GetChildren() const {
    return children;
}

const int UIElement::Depth() const {
    return depth;
}

bool UIElement::OnClick(const vec2& clickedPoint) {
    return false;
}

void UIElement::Render() {
    this->Renderable::Render();
    for (int i = 0; i < children.size(); i++)
        children[i]->Render();
}