#pragma once
#include <glm/glm.hpp>

#define ZOOM_SPEED 1.5f
#define MIN_ZOOM ZOOM_SPEED / 8.f
#define MAX_ZOOM ZOOM_SPEED * 8.f

class Camera final {
public:
    Camera(float fov, int worldViewportWidth, int worldViewportHeight, bool orthographic);

    const glm::mat4& GetProjection() const;
    const glm::mat4 GetTransform() const;

    Camera& Rotate(const glm::vec3& axis, float angle);
    Camera& Translate(const glm::vec3& translation);
    Camera& Zoom(float zoomDelta);
    const glm::vec3& GetZoom() const;

    Camera& SetOrthographic(bool ortho);
private:
    bool orthographic = false;
    glm::mat4 projection;
    glm::vec3 translation, scale;
    glm::vec4 rotation;
    float fov = 45.f;
    int worldViewportWidth = 1280, worldViewportHeight = 720;
    float zoom = 1.f;
private:
    void CalculateProjection();
};