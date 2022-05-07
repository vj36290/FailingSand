#include "Shaders.hpp"

#include <iostream>
#include <fstream>

#define INFO_BUFFER_SIZE 512

namespace Shaders {
    const GLchar* vertexSource =
        "#version 330 core\n"

        "layout (location = 0) in vec3 position;\n"
        "layout (location = 1) in vec2 uv;\n"
        "layout (location = 2) in vec4 color;\n"

        "uniform mat4 proj;\n"
        "uniform mat4 view;\n"
        "uniform mat4 transform;\n"

        "out vec4 vertexColor;\n"
        "out vec2 uvs;\n"

        "void main() {\n"
        "    gl_Position = proj * view * transform * vec4(position.x, position.y, position.z, 1.0f);\n"
        "    vertexColor = color;\n"
        "    uvs = uv;\n"
        "}\n";

    const GLchar* fragmentSource =
        "#version 330 core\n"

        "in vec4 vertexColor;\n"
        "in vec2 uvs;\n"
        "uniform sampler2D tex;\n"

        "layout (location = 0) out vec4 color;\n"

        "void main() {\n"
        "    color = texture(tex, uvs) * vertexColor;\n"
        "}\n";

    const GLchar* uiVertexSource =
        "#version 330 core\n"

        "layout (location = 0) in vec2 position;\n"
        "layout (location = 1) in vec2 uv;\n"
        "layout (location = 2) in vec4 color;\n"

        "uniform vec4 colorMod = vec4(1.0);\n"
        "uniform mat4 proj;\n"
        "uniform mat4 transform;\n"

        "out vec4 vertexColor;\n"
        "out vec2 uvs;\n"

        "void main() {\n"
        "    gl_Position = proj * transform * vec4(position.x, position.y, 0.0f, 1.0f);\n"
        "    vertexColor = colorMod * color;\n"
        "}\n";

    const GLchar* uiFragmentSource =
        "#version 330 core\n"

        "in vec4 vertexColor;\n"
        "out vec4 color;\n"

        "in vec2 uvs;\n"
        "uniform sampler2D tex;\n"

        "void main() {\n"
        "    color = texture(tex, uvs) * vertexColor;\n"
        "}\n";

    GLuint defaultShaderProgram = GL_NONE;
    GLuint defaultUIShaderProgram = GL_NONE;

    GLuint currentWorldShader = GL_NONE;
    GLuint currentUIShader = GL_NONE;

    std::map<std::string, Shader> shaderPrograms = std::map<std::string, Shader>();

    Shader create(const GLchar* vertexSource, const GLchar* fragmentSource, const std::string& name) {
        // vertex shader
        GLint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexSource, NULL);
        glCompileShader(vertexShader);

        GLchar infoLog[INFO_BUFFER_SIZE];
        GLint success;
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vertexShader, INFO_BUFFER_SIZE, NULL, infoLog);
            printf("An error occurred while compiling the vertex shader.\n % s\n", infoLog);
        }

        // fragment shader
        GLint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
        glCompileShader(fragmentShader);
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fragmentShader, INFO_BUFFER_SIZE, NULL, infoLog);
            printf("An error occurred while compiling the fragment shader.\n%s\n", infoLog);
        }

        GLuint shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shaderProgram, INFO_BUFFER_SIZE, NULL, infoLog);
            printf("Failed to link shader.\n%s\n", infoLog);
        }

        // free memory
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        return Shader(name, shaderProgram);
    }

    Shader create(const std::string& vertexFile, const std::string& fragmentFile, const std::string& name) {
        return Shader(name, Shaders::defaultShaderProgram);
    }
}

Shader::Shader(const std::string& name, const GLuint& program) : name(name), program(program) {
    Shaders::shaderPrograms.insert(std::make_pair(name, *this));
}
