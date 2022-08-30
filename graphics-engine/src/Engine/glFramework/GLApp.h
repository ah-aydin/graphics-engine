#pragma once

#include <Engine/BaseApp.h>
#include "GLWindow.h"

class GLApp : public BaseApp
{
public:
    GLApp() : window("OpenGL App") {};
    ~GLApp();

private:
    // Initialization
    bool init() override;
    void initGlProperties();
    void initInput() override;
    // Terminate
    void quit() override;

    // Main loop
    void mainLoop() override;

public:
    int run() override;

    GLWindow window;
};