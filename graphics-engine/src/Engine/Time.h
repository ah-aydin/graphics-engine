#pragma once

class Time
{
private:
    static double s_TimeCurrentFrame;
    static double s_TimeLastFrame;
    static double deltaTime;
public:
    // Returns the time in miliseconds since the last frame
    static double getDeltaTime() { return Time::deltaTime; }
    static void tick();
};