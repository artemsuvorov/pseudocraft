#pragma once

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Application
{
public:
    Application();
    ~Application();
public:
    void Start();

protected:
    virtual void OnInit() = 0;
    virtual void OnRender() = 0;

private:
    GLFWwindow* window;
private:
    void InitializeGlfw();
    void CreateGlfwWindow();
    void InitializeGlad();
    void TerminateGlfw();
private:
    const uint32_t ERROR_CODE_FAILURE = -1;
};