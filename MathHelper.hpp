#pragma once

#include <glm/glm.hpp>

// Linearly interpolates between the provided floating point numbers.
inline float Lerp(float a, float b, float ratio) {
    return a + (b - a) / ratio;
}

// Linearly interpolates between two 4x4 matrices.
inline glm::mat4 LerpMatrix(glm::mat4 a, glm::mat4 b, float ratio) {
    glm::mat4 matrix = glm::mat4(0.f);
    for (int i = 0; i < 4; i++) {
        for (int k = 0; k < 4; k++) {
            matrix[i][k] = Lerp(a[i][k], b[i][k], ratio);
        }
    }
    return matrix;
}