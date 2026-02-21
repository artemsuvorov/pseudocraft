#include <iostream>
#include <array>

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "core/log.h"
#include "core/application.h"

#include "graphics/graphics.h"
#include "graphics/shader.h"


struct Camera
{
    glm::vec3 Pos = glm::vec3(2.0f, 2.0f, 2.0f);
    glm::vec3 Dir = glm::vec3(-0.577f, -0.577f, -0.577f); // normalized direction from pos to origin initially
    glm::vec3 Up  = glm::vec3(0.0f, 1.0f, 0.0f);
    float Yaw     = -135.0f;      // degrees, so that initial Front points approximately to origin
    float Pitch   = -35.0f;       // degrees
    float Sens    = 0.1f;         // rotation sensitivity
    float Speed   = 2.5f;         // movement speed (units per second)
};


class Pseudocraft : public Application
{
public:
    Pseudocraft(const char* title, int32_t w, int32_t h) : Application(title, w, h) {}
    virtual ~Pseudocraft() = default;

protected:
    virtual void OnInit()
    {
        std::array positions = {
            // front face (z = +0.5)
            -0.5f, -0.5f,  0.5f,  // 0
             0.5f, -0.5f,  0.5f,  // 1
             0.5f,  0.5f,  0.5f,  // 2
            -0.5f,  0.5f,  0.5f,  // 3
            // back face (z = -0.5)
            -0.5f, -0.5f, -0.5f,  // 4
             0.5f, -0.5f, -0.5f,  // 5
             0.5f,  0.5f, -0.5f,  // 6
            -0.5f,  0.5f, -0.5f   // 7
        };
        
        std::array indices = {
            // front face
            0, 1, 2,  2, 3, 0,
            // back face
            4, 6, 5,  6, 4, 7,   // note: reversed order because of orientation (counter‑clockwise)
            // left face
            4, 0, 3,  3, 7, 4,
            // right face
            1, 5, 6,  6, 2, 1,
            // top face
            3, 2, 6,  6, 7, 3,
            // bottom face
            4, 5, 1,  1, 0, 4,
        };

        // Create and bind a Vertex Array Object (VAO)
        // In modern OpenGL, a VAO must be bound before setting vertex attributes.
        GL_CALL(glGenVertexArrays(1, &m_VertexArray));
        GL_CALL(glBindVertexArray(m_VertexArray));

        // Create and fill the Vertex Buffer Object (VBO)
        m_VertexBuffer = SelectBuffer(GL_ARRAY_BUFFER, positions, GL_STATIC_DRAW);

        // Define the layout of the vertex data (position attribute)
        GL_CALL(glEnableVertexAttribArray(0));
        GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0));

        // Create and fill the Element Buffer Object (EBO)
        m_IndexBuffer = SelectBuffer(GL_ELEMENT_ARRAY_BUFFER, indices, GL_STATIC_DRAW);

        // Load and compile shaders, then use the program
        ShaderSource source = ParseShader("res/shaders/cube.shader");
        m_Shader = CreateShader(source.VertexSource, source.FragmentSource);
        GL_CALL(glUseProgram(m_Shader));

        GL_CALL(glEnable(GL_DEPTH_TEST));
        GL_CALL(glEnable(GL_CULL_FACE));

        GL_CALL(glBindVertexArray(0));
    }

    virtual void OnUpdate()
    {
        constexpr float fov = glm::radians(45.0f);
        const float aspect = (float)m_WindowWidth / m_WindowHeight;
        const float znear = 0.1f, zfar = 100.0f;

        UpdateCamera();

        // Projection matrix
        glm::mat4 projection = glm::perspective(fov, aspect, 0.1f, 100.0f);
        // View matrix
        glm::mat4 view = glm::lookAt(m_Camera.Pos, m_Camera.Pos + m_Camera.Dir, m_Camera.Up);
        // Cube rotation
        m_Cube.Angle += 0.01f;
        glm::mat4 model = glm::rotate(glm::mat4(1.0f), m_Cube.Angle, m_Cube.Axis);
        glm::mat4 mvp = projection * view * model;

        // Upload to shader
        GL_CALL(glUseProgram(m_Shader));
        int32_t umvp = glGetUniformLocation(m_Shader, "u_ViewProjection");
        GL_CALL(glUniformMatrix4fv(umvp, 1, GL_FALSE, glm::value_ptr(mvp)));
        int32_t color = glGetUniformLocation(m_Shader, "u_Color");
        GL_CALL(glUniform3f(color, m_Cube.Color.r, m_Cube.Color.g, m_Cube.Color.b));
    }

    void UpdateCamera()
    {
        // Update rotation angles yaw and pitch
        m_Camera.Yaw = m_Camera.Yaw + m_MouseDeltaX * m_Camera.Sens;
        m_Camera.Pitch = glm::clamp(m_Camera.Pitch + m_MouseDeltaY * m_Camera.Sens, -89.0f, +89.0f);

        // Update front vector based on yaw and pitch
        glm::vec3 front;
        front.x = cos(glm::radians(m_Camera.Yaw)) * cos(glm::radians(m_Camera.Pitch));
        front.y = sin(glm::radians(m_Camera.Pitch));
        front.z = sin(glm::radians(m_Camera.Yaw)) * cos(glm::radians(m_Camera.Pitch));
        m_Camera.Dir = glm::normalize(front);

        // Update camera position
        float forwardInput = (float)(m_Keys[GLFW_KEY_W]) - (float)(m_Keys[GLFW_KEY_S]);
        float sideInput = (float)(m_Keys[GLFW_KEY_D]) - (float)(m_Keys[GLFW_KEY_A]);
        float upInput = (float)(m_Keys[GLFW_KEY_SPACE]) - (float)(m_Keys[GLFW_KEY_LEFT_SHIFT]);
        glm::vec3 forward = m_Camera.Dir;
        glm::vec3 side = glm::normalize(glm::cross(m_Camera.Dir, m_Camera.Up));
        glm::vec3 up = m_Camera.Up;
        glm::vec3 movement = forwardInput * forward + sideInput * side + upInput * up;
        if (glm::length(movement) > 0.0f)
            m_Camera.Pos += glm::normalize(movement) * m_Camera.Speed * m_DeltaTime;
    }

    virtual void OnRender()
    {
        GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
        GL_CALL(glBindVertexArray(m_VertexArray));
        GL_CALL(glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr));
    }

    virtual void OnDestroy()
    {
        GL_CALL(glDeleteProgram(m_Shader));
        GL_CALL(glDeleteVertexArrays(1, &m_VertexArray));
        GL_CALL(glDeleteBuffers(1, &m_VertexBuffer));
        GL_CALL(glDeleteBuffers(1, &m_IndexBuffer));
    }

private:
    Camera m_Camera;

    struct Cube {
        float Angle = 0.0f;
        glm::vec3 Axis = glm::vec3(0.5f, 1.0f, 0.3f);
        glm::vec3 Color = glm::vec3(0.2f, 0.2f, 0.2f);
    } m_Cube;

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
