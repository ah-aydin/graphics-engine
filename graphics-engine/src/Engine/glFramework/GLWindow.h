#pragma once

#include <Engine/BaseWindow.h>

class GLWindow : public BaseWindow
{
public:
    GLWindow(const char* title) : BaseWindow(title) {};
    ~GLWindow();

    bool init() override;

    // Call this on the update functino to switch screen buffers
    void tick() override;

private:
    void setWindowHints() override;
};