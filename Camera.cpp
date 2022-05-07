#include "Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

using namespace glm;

Camera::Camera(float fov, int worldViewportWidth, int worldViewportHeight, bool orthographic) {
    this->fov = fov;
    this->orthographic = orthographic;
    this->worldViewportWidth = worldViewportWidth;
    this->worldViewportHeight = worldViewportHeight;

    translation = vec3(0.f);
    rotation = vec4(0.f);
    scale = vec3(1.f);

    CalculateProjection();
}

const mat4& Camera::GetProjection() const {
    return projection;
}

const mat4 Camera::GetTransform() const {
    return
        glm::scale(glm::translate(mat4(1.f), translation), scale);
}

Camera& Camera::Rotate(const vec3& axis, float angle) {
    //transform = rotate(transform, angle, axis);
    return *this;
}

Camera& Camera::Translate(const vec3& translation) {
    this->translation += translation;
    return *this;
}

Camera& Camera::Zoom(float zoomDelta) {
    zoomDelta = powf(ZOOM_SPEED, zoomDelta);
    zoom *= zoomDelta;
    if (zoom < MIN_ZOOM)
        zoom = MIN_ZOOM;
    else if (zoom > MAX_ZOOM)
        zoom = MAX_ZOOM;
    scale = vec3(zoom, zoom, zoom);
    return *this;
}

const vec3& Camera::GetZoom() const {
    return scale;
}

Camera& Camera::SetOrthographic(bool ortho) {
    bool shouldUpdate = orthographic != ortho;
    orthographic = ortho;
    // only recalculate if necessary
    if (shouldUpdate)
        CalculateProjection();
    return *this;
}

void Camera::CalculateProjection() {
    if (orthographic) {
        float top = -worldViewportHeight / 2.f;
        float bottom = -top;
        float right = worldViewportWidth / 2.f;
        float left = -right;
        projection = glm::ortho(left, right, bottom, top, -1.f, 1.f);
    }
    else {
        fov = fov * (180.f / pi<float>());
        float aspect = worldViewportWidth / worldViewportHeight;
        projection = glm::perspective(fov, aspect, 1e-3f, 1e5f);
    }
}