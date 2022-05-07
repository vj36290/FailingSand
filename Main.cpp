#include "FallingSandEngine.hpp"
#include "Shaders.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/GL.h>
#include <GLFW/glfw3.h>

#include <iostream>

// temp shit
unsigned int framesPerSecond = 30;
double lastUpdateTime = 0.0;
double updateInterval = 1.0 / framesPerSecond;
double deltaTime = updateInterval;

extern "C" {
    void reshape(GLFWwindow* win, int width, int height) {
        FallingSandEngine* app = (FallingSandEngine*)glfwGetWindowUserPointer(win);

        bool clamped = false;
        if (width < app->minWindowWidth) {
            width = app->minWindowWidth;
            clamped = true;
        }
        if (height < app->minWindowHeight) {
            height = app->minWindowHeight;
            clamped = true;
        }
        if (clamped)
            glfwSetWindowSize(win, width, height);

        glfwGetFramebufferSize(win, &app->windowWidth, &app->windowHeight);

        // update viewport size
        glViewport(0, 0, app->windowWidth, app->windowHeight);
    }

    void mousePress(GLFWwindow* win, int button, int action, int mods) {
        FallingSandEngine* app = (FallingSandEngine*)glfwGetWindowUserPointer(win);
        if (action == GLFW_PRESS)
            app->OnMouseDown(button, mods);
        else if (action == GLFW_RELEASE)
            app->OnMouseUp(button, mods);
    }

    void mouseMove(GLFWwindow* win, double x, double y) {
        FallingSandEngine* app = (FallingSandEngine*)glfwGetWindowUserPointer(win);
        app->OnMouseMove(x, y);
    }

    void mouseScroll(GLFWwindow* win, double xOffset, double yOffset)
    {
        FallingSandEngine* app = (FallingSandEngine*)glfwGetWindowUserPointer(win);
        app->OnMouseScroll(xOffset, yOffset);
    }

    void keyPress(GLFWwindow* win, int key, int scancode, int action, int mods) {
        FallingSandEngine* app = (FallingSandEngine*)glfwGetWindowUserPointer(win);
        if (action == GLFW_PRESS)
            app->OnKeyDown(key, scancode, mods);
        else if (action == GLFW_RELEASE)
            app->OnKeyUp(key, scancode, mods);
    }
}

int main(int argc, char* argv[]) {
    FallingSandEngine app = FallingSandEngine();

    GLFWwindow* win = app.win;
    glfwSetWindowUserPointer(win, &app);

    glfwSetFramebufferSizeCallback(win, reshape);
    glfwSetKeyCallback(win, keyPress);
    glfwSetMouseButtonCallback(win, mousePress);
    glfwSetCursorPosCallback(win, mouseMove);
    glfwSetScrollCallback(win, mouseScroll);

    reshape(win, app.windowWidth, app.windowHeight);

    glViewport(0, 0, app.windowWidth, app.windowHeight);

    Shaders::defaultShaderProgram = Shaders::create(
        Shaders::vertexSource, Shaders::fragmentSource, "default_shader")
        .program;

    Shaders::defaultUIShaderProgram = Shaders::create(
        Shaders::uiVertexSource, Shaders::uiFragmentSource, "default_ui"
    ).program;

    Shaders::currentWorldShader = Shaders::defaultShaderProgram;
    Shaders::currentUIShader = Shaders::defaultUIShaderProgram;

    /*GLuint shaderProgram = Shaders::defaultShaderProgram;
    glUseProgram(shaderProgram);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);*/

    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);

    app.Init();

    while (!glfwWindowShouldClose(win)) {

        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        app.RenderWorld();
        app.RenderUI();

        glfwSwapBuffers(win);

        glfwPollEvents();
        app.Update(deltaTime);

        // we stall
        while (lastUpdateTime + updateInterval > glfwGetTime()) {
        }
        deltaTime = glfwGetTime() - lastUpdateTime;
        lastUpdateTime = glfwGetTime();
    }

    return 0;
}