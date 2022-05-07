#include "FallingSandEngine.hpp"

#include "Cell.hpp"
#include "UIBox.hpp"
#include "UILabel.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <GL/GL.h>

#include <algorithm>
#include <iostream>

#ifndef F_PI
#define F_PI 3.1415926f
#endif

#ifndef F_PI_2
#define F_PI_2 2 * F_PI
#endif

using namespace glm;

FallingSandEngine::FallingSandEngine() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

    win = glfwCreateWindow(windowWidth, windowHeight, windowTitle.c_str(), 0, 0);
    assert(win);
    glfwMakeContextCurrent(win);
    
    monitor = glfwGetPrimaryMonitor();

    glewExperimental = true;
    glewInit();
}

FallingSandEngine::~FallingSandEngine() {
    glfwDestroyWindow(win);
    glfwTerminate();

    if (uiRoot)
        delete uiRoot;
    if (uiCamera)
        delete uiCamera;
    if (worldCamera)
        delete worldCamera;
    if (world)
        delete world;
}

void FallingSandEngine::Init() {
    // world setup
    if (worldCamera)
        delete worldCamera;
    worldCamera = new Camera(45.f, worldViewportWidth, worldViewportHeight, true);

    if (world)
        delete world;
    world = new World(this);

    // UI setup
    if (uiCamera)
        delete uiCamera;
    // UI needs to be flipped vertically due to screen coords vs world coords
    uiCamera = new Camera(45.f, uiViewportWidth, -uiViewportHeight, true);

    if (uiRoot)
        delete uiRoot;
    uiRoot = new UIBox(this, "uiBorderTop", nullptr,
        vec2(0.f, 0.f),
        vec2(uiViewportWidth, uiBorderWidth),
        vec2(0.f, 0.f),
        vec2(0.f, 0.f),
        2.f,
        vec4(0.6f, 0.6f, 0.6f, 1.f),
        vec4(0.f, 0.f, 0.f, 0.f));

    new UIBox(this, "uiBorderBottom", uiRoot,
        vec2(0.f, uiViewportHeight - uiBorderWidth),
        vec2(uiViewportWidth, uiBorderWidth),
        vec2(0.f, 0.f),
        vec2(0.f, 0.f),
        2.f,
        vec4(0.6f, 0.6f, 0.6f, 1.f),
        vec4(0.f, 0.f, 0.f, 0.f));

    new UIBox(this, "uiBorderLeft", uiRoot,
        vec2(0.f, 0.f),
        vec2(uiBorderWidth, uiViewportHeight),
        vec2(0.f, 0.f),
        vec2(0.f, 0.f),
        2.f,
        vec4(0.6f, 0.6f, 0.6f, 1.f),
        vec4(0.f, 0.f, 0.f, 0.f));

    new UIBox(this, "uiBorderRight", uiRoot,
        vec2(uiViewportWidth - uiBorderWidth, 0.f),
        vec2(uiBorderWidth, uiViewportHeight),
        vec2(0.f, 0.f),
        vec2(0.f, 0.f),
        2.f,
        vec4(0.6f, 0.6f, 0.6f, 1.f),
        vec4(0.f, 0.f, 0.f, 0.f));

    materialDisplay = new UIBox(this, "material", uiRoot,
        vec2(5, uiViewportHeight - (uiBorderWidth - 5)),
        vec2(uiBorderWidth - 10, uiBorderWidth - 10),
        vec2(0.0f, 0.0f),
        vec2(0.f, 0.f),
        2.f,
        vec4(1.f),
        vec4(1.f, 1.f, 1.f, 1.f));

    brush = new UIBrush(this);

    int materials = Materials::Instance().NumMaterials();
    // start on first defined material if applicable,
    // otherwise none (none should always be hardcoded)
    SetActiveMaterial(materials > 1 ? 1 : 0);
}

void FallingSandEngine::RenderWorld() {
    // draw world here
    if (world)
        world->Render();
    if (brush)
        brush->Render();
}

void FallingSandEngine::RenderUI() {
    // todo: resize camera lmao
    if (uiRoot)
        uiRoot->Render();
}

void FallingSandEngine::Update(double deltaTime) {
    double time = glfwGetTime();

    vec2 mouse = vec2(mouseX, mouseY);

    if (mouseButtonDown > MOUSE_UP && mouseButtonDown < GLFW_MOUSE_BUTTON_3) {
        if (world) {
            double dy = mouseY - lastMouseY,
                dx = mouseX - lastMouseX,
                max = std::max(abs(dx), abs(dy));
            dy /= max, dx /= max;

            // establish start/end points
            vec2 p0, p1;

            if (lastMouseX <= mouseX) {
                p0.x = lastMouseX;
                p1.x = mouseX;
            }
            else {
                p0.x = mouseX;
                p1.x = lastMouseX;
            }

            if (lastMouseY <= mouseY) {
                p0.y = lastMouseY;
                p1.y = mouseY;
            }
            else {
                p0.y = mouseY;
                p1.y = mouseY;
            }

            vec2 p = vec2(p0);
            vec2 delta = vec2(dx, dy);

            while (p.x <= p1.x && p.y <= p1.y) {
                // due to some GLFW mouse polling fuckery, need to do a safety check
                    // basically just make sure deltas are pointing
                    // in the correct directions
                if (p1.x > p.x && delta.x < 0)
                    delta.x *= -1;
                if (p1.y > p.y && delta.y < 0)
                    delta.y *= -1;

                for (unsigned int x = 0; x < brushSize; x++) {
                    for (unsigned int y = 0; y < brushSize; y++) {
                        vec3 w = WindowToWorldPoint(p);
                        // we're moving in world units now, not screen units
                        w += vec3(x, y, 0.f);
                        Cell* cell = world->GetCell(w.x, w.y);
                        if (cell) {
                            cell->SetMaterial(mouseButtonDown ? 0 : material);
                        }
                    }
                }

                p += delta;
            }
        }
    }

    if (world)
        world->Update(deltaTime);

    lastMouseX = mouseX, lastMouseY = mouseY;
    lastUpdateTime = time;
}

Camera* FallingSandEngine::GetWorldCamera() {
    return worldCamera;
}

Camera* FallingSandEngine::GetUICamera() {
    return uiCamera;
}

void FallingSandEngine::OnKeyDown(int key, int scancode, int mods) {
    switch (key) {
    case GLFW_KEY_R:
        world->Reset((mods & GLFW_MOD_SHIFT) > 0);
        break;
    case GLFW_KEY_LEFT_SHIFT:
        modifiers += GLFW_MOD_SHIFT;
        break;
    case GLFW_KEY_LEFT_CONTROL:
        modifiers += GLFW_MOD_CONTROL;
        break;
    case GLFW_KEY_F11:
        SetFullScreen(!fullscreen);
        break;
    default:
        break;
    }
}

void FallingSandEngine::OnKeyUp(int key, int scancode, int mods) {
    switch (key) {
    case GLFW_KEY_LEFT_SHIFT:
        modifiers -= GLFW_MOD_SHIFT;
        break;
    case GLFW_KEY_LEFT_CONTROL:
        modifiers -= GLFW_MOD_CONTROL;
        break;
    default:
        break;
    }
}

void traverse(UIElement* cur, const vec2& point) {
    std::vector<UIElement*> children = cur->GetChildren();
    for (int i = 0; i < children.size(); i++) {
        traverse(children[i], point);
    }
    if (cur->Contains(point))
        cur->OnClick(point);
}

void FallingSandEngine::OnMouseDown(int button, int mods) {
    glfwGetCursorPos(win, &mouseX, &mouseY);

    mouseButtonDown = button;

    // check UI first
    vec2 mouse = vec2(mouseX, mouseY);
    vec2 screenPoint = WindowToScreenPoint(mouse);

    if (uiRoot)
        traverse(uiRoot, screenPoint);
}

void FallingSandEngine::OnMouseUp(int button, int mods) {
    glfwGetCursorPos(win, &mouseX, &mouseY);
    mouseButtonDown = MOUSE_UP;
}

void FallingSandEngine::OnMouseMove(double x, double y) {
    // 3d code that isn't getting used lmao
    //float yaw = float(F_PI * float(x - mouseX) / windowWidth);
    //float pitch = float(F_PI * float(y - mouseY) / windowHeight);

    if (worldCamera && mouseButtonDown == GLFW_MOUSE_BUTTON_3) {
        float dX = (x - mouseX), dY = (y - mouseY);
        dX = (dX / windowWidth) * worldViewportWidth,
            dY = (dY / windowHeight) * worldViewportHeight;
        worldCamera->Translate(vec3(dX, dY, 0.f));
    }

    mouseX = x;
    mouseY = y;
}

void FallingSandEngine::OnMouseScroll(double xOffset, double yOffset) {
    if (yOffset) {
        int dir = yOffset / abs(yOffset);
        if ((modifiers & GLFW_MOD_SHIFT)) {
            SetActiveMaterial(material + dir);
        }
        else if ((modifiers & GLFW_MOD_CONTROL)) {
            brushSize = std::max(std::min(brushSize + dir, maxBrushSize), minBrushSize);
        }
        else if (worldCamera) {
            worldCamera->Zoom(yOffset);
        }
    }
}

vec2 FallingSandEngine::WindowToScreenPoint(const vec2& windowPos) const {
    // ex. window width of 400 = [-200, 200]
    // viewport width of 20 = [-10, 10]
    // mouse X is 100 so we want it at 5 in the viewport
    // (400 / 2) - 100 = 200 - 100 = 100 / (200) = 0.5 * 10 = 5
    float x = (windowPos.x - (windowWidth / 2.f)) / (windowWidth);
    float y = (windowPos.y - (windowHeight / 2.f)) / (windowHeight);
    return vec2(x * uiViewportWidth, y * uiViewportHeight);
}

vec3 FallingSandEngine::WindowToWorldPoint(const glm::vec2& windowPos) const {
    return glm::unProject(vec3(windowPos.x, windowHeight - windowPos.y, 0.f),
        worldCamera->GetTransform(),
        worldCamera->GetProjection(),
        vec4(0, 0, windowWidth, windowHeight));
}

ivec2 FallingSandEngine::GetMousePosition() const {
    return ivec2(mouseX, mouseY);
}

unsigned int FallingSandEngine::GetBrushSize() const {
    return brushSize;
}

bool FallingSandEngine::isFullScreen() const {
    return fullscreen;
}

void FallingSandEngine::SetFullScreen(bool fullscreen) {
    // don't bother with the below if we're already fullscreen
    if (this->fullscreen != fullscreen) {
        this->fullscreen = fullscreen;
        if (fullscreen) {
            const GLFWvidmode* mode = glfwGetVideoMode(monitor);
            glfwGetWindowPos(win, &windowPosition.x, &windowPosition.y);
            glfwGetWindowSize(win, &windowSize.x, &windowSize.y);
            glfwSetWindowMonitor(win, monitor, 
                0, 0, 
                mode->width, mode->height, 
                0);
        }
        else {
            glfwSetWindowMonitor(win, nullptr, 
                windowPosition.x, windowPosition.y,
                windowSize.x, windowSize.y, 0);
        }
    }
}

void FallingSandEngine::SetActiveMaterial(unsigned int material) {
    material = abs(material % Materials::Instance().NumMaterials());
    this->material = material;
    vec4 matColor;
    const unsigned char* color = Materials::Instance().Get(material)->texture.data();
    for (int i = 0; i < 4; i++) {
        matColor[i] = (float)color[i] / 255.f;
    }
    std::cout << std::endl;
    materialDisplay->colorMod = matColor;
}