#pragma once

#include "input.h"


// Forwards.
struct GLFWwindow;


class Application
{
public:
    Application(const char* title, int32_t width, int32_t height);
    virtual ~Application();

    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;
    Application(Application&&) = delete;
    Application& operator=(Application&&) = delete;

    void Start();
    void Close();

protected:
    virtual void OnInit() = 0;
    virtual void OnUpdate() = 0;
    virtual void OnRender() = 0;
    virtual void OnDestroy() = 0;

    virtual void OnMouseEvent(float x, float y) {}
    virtual void OnKeyboardEvent(int32_t key, int32_t action) {}

    const char* m_Title = nullptr;
    int32_t m_WindowWidth = 0;
    int32_t m_WindowHeight = 0;
 
    float m_DeltaTime = 0.0f;
    float m_LastTime = 0.0f;
    Input m_Input;

    GLFWwindow* m_Window = nullptr;

private:
    void InitializeGlfw();
    void CreateGlfwWindow();
    void RegisterGlfwCallbacks();
    void InitializeGlad();
    void TerminateGlfw();

    const uint32_t ERROR_CODE_FAILURE = -1;
};