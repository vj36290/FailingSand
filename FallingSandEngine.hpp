#pragma once

#include "Camera.hpp"
#include "World.hpp"
#include "UIElement.hpp"
#include "UIBox.hpp"
#include "UIBrush.hpp"

#ifndef MOUSE_UP
#define MOUSE_UP -1
#endif

class FallingSandEngine final {
public:
    const std::string windowTitle = "Failing Sand";
    const float aspectRatio = 16.f / 9.f;
    const int minWindowHeight = 720, minWindowWidth = minWindowHeight * aspectRatio;

    struct GLFWwindow* win;
    struct GLFWmonitor* monitor;

    bool windowActive = false;
    int windowWidth = 1280, windowHeight = 720;

    // world sizes
    const int worldViewportHeight = 128, 
        worldViewportWidth = worldViewportHeight * aspectRatio;
    
    // ui sizes
    const int uiViewportWidth = windowWidth, uiViewportHeight = windowHeight;
    const int uiBorderWidth = 50;
public:
    FallingSandEngine();
    ~FallingSandEngine();

    void Init();
    void RenderWorld();
    void RenderUI();
    void Update(double deltaTime);

    Camera* GetWorldCamera();
    Camera* GetUICamera();

    void OnKeyDown(int key, int scancode, int mods);
    void OnKeyUp(int key, int scancode, int mods);

    void OnMouseDown(int button, int mods);
    void OnMouseUp(int button, int mods);
    void OnMouseMove(double x, double y);
    void OnMouseScroll(double xOffset, double yOffset);

    glm::vec2 WindowToScreenPoint(const glm::vec2& windowPos) const;
    glm::vec3 WindowToWorldPoint(const glm::vec2& windowPos) const;

    glm::ivec2 GetMousePosition() const;
    unsigned int GetBrushSize() const;

    bool isFullScreen() const;
    void SetFullScreen(bool fullscreen);
private:
    Camera* worldCamera, *uiCamera;
    World* world;
    UIElement* uiRoot;
    UIBox* materialDisplay;
    UIBrush* brush;
    double lastUpdateTime = 0;
    double mouseX, mouseY, lastMouseX, lastMouseY;

    int mouseButtonDown = -1;
    int modifiers = 0;
    unsigned int material = 0, brushSize = 1, minBrushSize = 1, maxBrushSize = 8;

    bool fullscreen = false;
    
    glm::ivec2 monitorSize, windowPosition, windowSize;
private:
    void SetActiveMaterial(unsigned int material);
};