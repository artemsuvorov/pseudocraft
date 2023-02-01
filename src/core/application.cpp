#include "application.h"

Application::Application()
{
}

Application::~Application()
{
    TerminateGlfw();
}

void Application::Start()
{
    InitializeGlfw();
    CreateGlfwWindow();
    InitializeGlad();

    OnInit();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        OnRender();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
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
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Pseudocraft", nullptr, nullptr);
    
    if (window) 
    {
        /* Make the window's context current */
        glfwMakeContextCurrent(window);
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

void Application::TerminateGlfw()
{
    glfwTerminate();
}