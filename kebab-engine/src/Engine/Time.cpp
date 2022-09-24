#include "Time.h"

#include <GLFW/glfw3.h>

namespace kbb
{
    double Time::s_TimeCurrentFrame;
    double Time::s_TimeLastFrame;
    double Time::deltaTime = 0;

    // Call this function once in the main loop to calculate the time since last frame
    void Time::tick()
    {
        Time::s_TimeCurrentFrame = glfwGetTime();
        Time::deltaTime = (Time::s_TimeCurrentFrame - Time::s_TimeLastFrame) / 1000.f;
        Time::s_TimeLastFrame = Time::s_TimeCurrentFrame;
    }
}