#include <iostream>
#include <assert.h>

#include <glad/glad.h>

#include "application.h"


Application::Application(const char* title, int32_t width, int32_t height)
{
    m_Title = title;
    m_WindowWidth = width;
    m_WindowHeight = height;
}

Application::~Application()
{
    TerminateGlfw();
}

void Application::Start()
{
    InitializeGlfw();
    CreateGlfwWindow();
    RegisterGlfwCallbacks();
    InitializeGlad();

    OnInit();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(m_Window))
    {
        /* Update delta time */
        float currentFrame = (float)glfwGetTime();
        m_DeltaTime = currentFrame - m_LastTime;
        m_LastTime = currentFrame;

        /* Poll for and process events */
        glfwPollEvents();

        OnUpdate();

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        OnRender();

        /* Swap front and back buffers */
        glfwSwapBuffers(m_Window);

        /* Reset mouse's delta */
        m_Input.ResetMouse();
    }

    OnDestroy();
}

void Application::Close()
{
    glfwSetWindowShouldClose(m_Window, true);
}

void Application::InitializeGlfw()
{
    /* Initialize the library */
    if (glfwInit()) return;

    std::cout << "Failed to init the GLFW library." << std::endl;
    exit(ERROR_CODE_FAILURE);
}

void Application::CreateGlfwWindow()
{
    assert(m_Title && std::strlen(m_Title) > 0);
    assert(m_WindowWidth > 0 && m_WindowHeight > 0);

    /* Create a windowed mode window and its OpenGL context */
    m_Window = glfwCreateWindow(m_WindowWidth, m_WindowHeight, m_Title, nullptr, nullptr);

    if (m_Window)
    {
        /* Make the window's context current */
        glfwMakeContextCurrent(m_Window);
        /* Disable cursor and capture it */
        glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        /* Set up User pointer to this instance of the App */
        glfwSetWindowUserPointer(m_Window, this);
        return;
    }

    std::cout << "Failed to create the GLFW window." << std::endl;
    TerminateGlfw();
    exit(ERROR_CODE_FAILURE);
}

void Application::InitializeGlad()
{
    if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
        return;
    }

    std::cout << "Failed to initialize GLAD" << std::endl;
    exit(ERROR_CODE_FAILURE);
}

void Application::RegisterGlfwCallbacks()
{
    glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int32_t width, int32_t height)
    {
        if (Application* app = (Application*)glfwGetWindowUserPointer(window))
        {
            app->m_WindowWidth = width;
            app->m_WindowHeight = height;
        }
        glViewport(0, 0, width, height);
    });

    glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double x, double y)
    {
        if (Application* app = (Application*)glfwGetWindowUserPointer(window))
        {
            app->m_Input.UpdateMouse(x, y);
            app->OnMouseEvent(x, y);
        }
    });

    glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int32_t key, int32_t scancode, int32_t action, int32_t mods)
    {
        if (Application* app = (Application*)glfwGetWindowUserPointer(window))
        {
            if (key >= 0 && key <= GLFW_KEY_LAST)
            {
                app->m_Input.UpdateKeys(key, action);
                app->OnKeyboardEvent(key, action);
            }
        }
    });
}

void Application::TerminateGlfw()
{
    glfwTerminate();
}
