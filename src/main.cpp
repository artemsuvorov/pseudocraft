#include <iostream>
#include <array>

#include "core/log.h"
#include "core/application.h"

#include "graphics/graphics.h"
#include "graphics/shader.h"

class Pseudocraft : public Application
{
public:
    Pseudocraft(const char* title, int32_t w, int32_t h) : Application(title, w, h) {}
    virtual ~Pseudocraft() = default;

protected:
    virtual void OnInit()
    {
        std::array<float, 8> positions = {
            -0.5f, -0.5f, // 0
             0.5f, -0.5f, // 1
             0.5f,  0.5f, // 2
            -0.5f,  0.5f, // 3
        };
        
        std::array<uint32_t, 6> indices = {
            0, 1, 2,
            2, 3, 0
        };

        // Create and bind a Vertex Array Object (VAO)
        // In modern OpenGL, a VAO must be bound before setting vertex attributes.
        GL_CALL(glGenVertexArrays(1, &m_VertexArray));
        GL_CALL(glBindVertexArray(m_VertexArray));

        // Create and fill the Vertex Buffer Object (VBO)
        m_VertexBuffer = SelectBuffer(GL_ARRAY_BUFFER, positions, GL_STATIC_DRAW);

        // Define the layout of the vertex data (position attribute)
        GL_CALL(glEnableVertexAttribArray(0));
        GL_CALL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0));

        // Create and fill the Element Buffer Object (EBO)
        m_IndexBuffer = SelectBuffer(GL_ELEMENT_ARRAY_BUFFER, indices, GL_STATIC_DRAW);

        // Load and compile shaders, then use the program
        ShaderSource source = ParseShader("res/shaders/blue.shader");
        m_Shader = CreateShader(source.VertexSource, source.FragmentSource);
        GL_CALL(glUseProgram(m_Shader));

        // Get uniform and set aspect ratio of the Window
        int32_t aspect = glGetUniformLocation(m_Shader, "u_Aspect");
        glUniform1f(aspect, (float)m_WindowWidth / (float)m_WindowHeight);

        GL_CALL(glBindVertexArray(0));
    }

    virtual void OnRender()
    {
        GL_CALL(glBindVertexArray(m_VertexArray));
        GL_CALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
    }

    virtual void OnDestroy()
    {
        GL_CALL(glDeleteProgram(m_Shader));
        GL_CALL(glDeleteVertexArrays(1, &m_VertexArray));
        GL_CALL(glDeleteBuffers(1, &m_VertexBuffer));
        GL_CALL(glDeleteBuffers(1, &m_IndexBuffer));
    }

private:
    uint32_t m_Shader = 0;
    uint32_t m_VertexArray = 0;
    uint32_t m_VertexBuffer = 0;
    uint32_t m_IndexBuffer = 0;
};

int main()
{
    Pseudocraft app{ "Pseudocraft", 1200, 800 };
    app.Start();
}