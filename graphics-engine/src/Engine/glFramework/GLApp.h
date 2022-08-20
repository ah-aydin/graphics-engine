#pragma once

#include "GLWindow.h"

class GLApp
{
public:
    GLApp() {};
    ~GLApp();

private:
    // Initialization
    bool init();
    bool initGlfw();
    void initGlProperties();
    void inputInit();
    // Terminate
    void quit();

    // Main loop
    void mainLoop();
    bool running = true;

public:
    int run();

    GLWindow window;
};