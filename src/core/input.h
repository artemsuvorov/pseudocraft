#pragma once

#include <GLFW/glfw3.h>
#include <glm/fwd.hpp>


struct Input
{
public:
    struct Keys
    {
        bool IsPressed[GLFW_KEY_LAST] = { 0 };
    } Key;

    struct Mouse
    {
        float X = 0.0f;
        float Y = 0.0f;
        float DeltaX = 0.0f;
        float DeltaY = 0.0f;
        bool First = true;
    } Mouse;

    glm::vec2 GetMouseDelta() const;
    bool IsKeyPressed(int32_t key) const;

    void UpdateMouse(float x, float y);
    void ResetMouse();
    void UpdateKeys(int32_t key, int32_t action);
};
