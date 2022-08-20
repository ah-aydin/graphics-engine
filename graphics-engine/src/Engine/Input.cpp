#include "Input.h"

#include <iostream>
#include <math.h>

#include <Logging/Log.h>

Mouse Input::mouse = { 0, 0 };
bool Input::keys[KEY_ARRAY_SIZE + MOUSE_KEY_ARRAY_SIZE] = { 0 };
std::map<std::string, InputAction> Input::actions;
std::map<std::string, InputAxis> Input::axes;

bool Input::getKeyPress(unsigned code, bool mouse)
{
    if (mouse) {
        code += KEY_ARRAY_SIZE;
    }
    return Input::keys[code];
}

void Input::keyPress(unsigned code, bool mouse)
{
    if (mouse) {
        code += KEY_ARRAY_SIZE;
    }
    keys[code] = true;
}

void Input::keyRelease(unsigned code, bool mouse)
{
    if (mouse) {
        code += KEY_ARRAY_SIZE;
    }
    keys[code] = false;
}

void Input::mouseMotion(double newX, double newY)
{
    Input::mouse.lastX = Input::mouse.currentX;
    Input::mouse.lastY = Input::mouse.currentY;
    Input::mouse.currentX = newX;
    Input::mouse.currentY = newY;
}

void Input::resetMouse()
{
    Input::mouse.lastX = Input::mouse.currentX;
    Input::mouse.lastY = Input::mouse.currentY;
}

void Input::createAction(const std::string& name, unsigned keyCode, bool mouse)
{
    if (mouse) {
        keyCode += KEY_ARRAY_SIZE;
    }
    Input::actions.insert(
        std::pair<std::string, InputAction>(name, { keyCode }));
}

void Input::createAxis(const std::string& name, unsigned positiveKey, unsigned negativeKey, bool positiveMouse, bool negativeMouse)
{
    if (positiveMouse) {
        positiveKey += KEY_ARRAY_SIZE;
    }
    if (negativeMouse) {
        negativeKey += KEY_ARRAY_SIZE;
    }
    Input::axes.insert(
        std::pair<std::string, InputAxis>(name, { positiveKey, negativeKey }));
}

bool Input::getAction(const std::string& action)
{
    try
    {
        // See if the action key is pressed
        return Input::getKeyPress(Input::actions.at(action).keyCode);
    }
    catch (const std::exception& e) // Tried to access an action that does not exist
    {
        log_error("ERROR::INPUT::ACTION_DOES_NOT_EXIST::%s::%s", action, e.what());
        return false;
    }
    return false;
}

float Input::getAxis(const std::string& axis)
{
    try
    {
        // Get axis and get check if it's keys are pressed
        auto ax = Input::axes.at(axis);
        bool pInput = Input::getKeyPress(ax.positiveKey);
        bool nInput = Input::getKeyPress(ax.negativeKey);

        if (pInput && nInput)   return 0.0f;    // Both pressed
        else if (pInput)        return 1.0f;    // Positiveo only
        else if (nInput)        return -1.0f;   // Negative only
        else                    return 0.0f;    // No presses
    }
    catch (const std::exception& e) // Tried to access an axis that does not exist
    {
        log_error("ERROR::INPUT::AXIS_DOES_NOT_EXIST::%s::%s", axis, e.what());
        return 0.f;
    }
    return 0.f;
}

double Input::mouseX() { return Input::mouse.lastX - Input::mouse.currentX; }
double Input::mouseY() { return Input::mouse.lastY - Input::mouse.currentY; }