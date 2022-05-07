#pragma once

#define GLEW_STATIC
#include "gl/glew.h"
#include <map>
#include <string>

struct Shader {
    const std::string name;
    const GLuint program;
    Shader(const std::string& name, const GLuint& program);
};

namespace Shaders {
    extern const GLchar* vertexSource;
    extern const GLchar* fragmentSource;
    extern const GLchar* uiVertexSource;
    extern const GLchar* uiFragmentSource;

    extern GLuint defaultShaderProgram;
    extern GLuint defaultUIShaderProgram;
    extern GLuint currentWorldShader;
    extern GLuint currentUIShader;

    extern std::map<std::string, Shader> shaderPrograms;

    Shader create(const GLchar* vertex, const GLchar* fragment, const std::string& name = "shader");
    Shader create(const std::string& vertexFile, const std::string& fragmentFile, const std::string& name = "shader");
};