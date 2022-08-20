#pragma once

#include <map>
#include <string>

#define KEY_ARRAY_SIZE 350
#define MOUSE_KEY_ARRAY_SIZE 10

struct Mouse
{
    double lastX, lastY;
    double currentX, currentY;
};

struct InputAction
{
    unsigned keyCode;
};

struct InputAxis
{
    unsigned positiveKey;
    unsigned negativeKey;
};


class Input
{
private:
    static bool keys[KEY_ARRAY_SIZE + MOUSE_KEY_ARRAY_SIZE];
    static bool getKeyPress(unsigned code, bool mouse = false);

    static Mouse mouse;
    static std::map<std::string, InputAction> actions;
    static std::map<std::string, InputAxis> axes;

public:
    // Event handlers
    static void keyPress(unsigned code, bool mouse = false);
    static void keyRelease(unsigned code, bool mouse = false);
    static void mouseMotion(double newX, double newY);
    static void resetMouse();

    // Input creation
    static void createAction(const std::string& name, unsigned keyCode, bool mouse = false);
    static void createAxis(const std::string& name, unsigned positiveKey, unsigned negativeKey, bool positiveMouse = false, bool negativeMouse = false);

    // Input access
    static bool getAction(const std::string& action);
    static float getAxis(const std::string& axis);
    static double mouseX();
    static double mouseY();
};