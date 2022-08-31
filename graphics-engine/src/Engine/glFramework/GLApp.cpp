#include "GLApp.h"

#ifdef GRAPHICS_API_OPENGL

#include <glad/glad.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <Engine/Input.h>
#include <Engine/Time.h>
#include <Engine/Settings.h>
#include <Logging/Log.h>

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
    if (!BaseApp::init()|| !window.init()) // intialize all components
        return false;

    initGlProperties();

    return true;
}

void GLApp::initGlProperties()
{
    glClearColor(0, 0, 0, 1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
}

void GLApp::quit()
{
    BaseApp::quit();
}

void GLApp::initInput()
{
    // Some inputs for testing
    Input::createAction("QUIT", GLFW_KEY_ESCAPE);
}

#include "Levels/LevelRenderTirangle.h"
#include "Levels/LevelVtxPulling.h"
#include "Levels/LevelCubemap.h"

void GLApp::mainLoop()
{
    //LevelRenderTriangle level;
    //LevelVtxPulling level;
    LevelCubemap level;
    if (!level.init())
    {
        return;
    }

    glViewport(0, 0, window.getWidth(), window.getHeight());
    Settings::ratio = (float)window.getWidth() / window.getHeight();

    while (running)
    {
        if (Input::getAction("QUIT")) {
            running = false;
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        level.update(Time::getDeltaTime());

        Input::resetMouse();
        Time::tick();
        window.tick();
    }

    window.setShouldClose(true);
}

#endif