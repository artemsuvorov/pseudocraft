#pragma once


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

protected:
    virtual void OnInit() = 0;
    virtual void OnUpdate() = 0;
    virtual void OnRender() = 0;
    virtual void OnDestroy() = 0;

    const char* m_Title = nullptr;
    int32_t m_WindowWidth = 0;
    int32_t m_WindowHeight = 0;

    GLFWwindow* m_Window = nullptr;

private:

    void InitializeGlfw();
    void CreateGlfwWindow();
    void InitializeGlad();
    void TerminateGlfw();

    const uint32_t ERROR_CODE_FAILURE = -1;
};