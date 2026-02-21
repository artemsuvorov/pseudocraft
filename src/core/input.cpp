#include <assert.h>

#include <glm/glm.hpp>

#include "input.h"


glm::vec2 Input::GetMouseDelta() const
{
    return glm::vec2(Mouse.DeltaX, Mouse.DeltaY);
}

bool Input::IsKeyPressed(int32_t key) const
{
    assert(key >= 0 && key <= GLFW_KEY_LAST);
    return Key.IsPressed[key];
}

void Input::UpdateMouse(float x, float y)
{
    if (Mouse.First)
    {
        Mouse.X = x;
        Mouse.Y = y;
        Mouse.First = false;
    }
    else
    {
        Mouse.DeltaX = x - Mouse.X;
        Mouse.DeltaY = Mouse.Y - y; // reversed Y
        Mouse.X = x;
        Mouse.Y = y;
    }
}

void Input::ResetMouse()
{
    Mouse.DeltaX = Mouse.DeltaY = 0.0f;
}

void Input::UpdateKeys(int32_t key, int32_t action)
{
    assert(key >= 0 && key <= GLFW_KEY_LAST);

    if (action == GLFW_PRESS)
        Key.IsPressed[key] = true;
    else if (action == GLFW_RELEASE)
        Key.IsPressed[key] = false;
}
