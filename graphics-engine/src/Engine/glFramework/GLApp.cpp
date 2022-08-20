#include "GLApp.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <Engine/Input.h>
#include <Engine/Time.h>
#include <Logging/Log.h>

#include "GLWindow.h"

GLApp::~GLApp()
{
    glfwTerminate();
}

int GLApp::run()
{
    // Initialize
    if (init() == false)
        return EXIT_FAILURE;

    // Run main loop
    mainLoop();

    quit();

    return EXIT_SUCCESS;
}

bool GLApp::init()
{
    log_reset();                                    // reset log file
    if (!initGlfw() || !window.init()) // intialize all components
        return false;

    inputInit(); // create sample input axes/actions

    return true;
}

bool GLApp::initGlfw()
{
    if (glfwInit() == GLFW_FALSE)
    {
        log_error("ERROR::GLFW::INITIALIZATION");
        return false;
    }
    return true;
}

void GLApp::quit()
{
    glfwTerminate();
}

void GLApp::inputInit()
{
    // Some inputs for testing
    Input::createAction("QUIT", GLFW_KEY_ESCAPE);
}

#include <glad/glad.h>
#include "Rendering/Shaders/GLProgram.h"
#include "Rendering/Shaders/GLShader.h"

void GLApp::mainLoop()
{
    GLShader shaderVertex("res/shader.vert");
    GLShader shaderFragment("res/shader.frag");
    GLProgram program(shaderVertex, shaderFragment);
    program.use();

    GLuint vao;
    glCreateVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glViewport(0, 0, window.getWidth(), window.getHeight());

    while (running)
    {
        if (Input::getAction("QUIT")) {
            running = false;
        }

        glClear(GL_COLOR_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        Input::resetMouse();
        Time::tick();
        window.tick();
    }
    glDeleteVertexArrays(1, &vao);

    window.setShouldClose(true);
}