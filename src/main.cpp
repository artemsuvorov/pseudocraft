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
#include "graphics/texture.h"


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
            -0.5f, -0.5f,  0.5f,   0.0f, 0.0f, // 0
             0.5f, -0.5f,  0.5f,   1.0f, 0.0f, // 1
             0.5f,  0.5f,  0.5f,   1.0f, 1.0f, // 2
            -0.5f,  0.5f,  0.5f,   0.0f, 1.0f, // 3
            // back face (z = -0.5)
            -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, // 4
             0.5f, -0.5f, -0.5f,   1.0f, 0.0f, // 5
             0.5f,  0.5f, -0.5f,   1.0f, 1.0f, // 6
            -0.5f,  0.5f, -0.5f,   0.0f, 1.0f, // 7
            // left face (x = -0.5)
            -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, // 8  (duplicate of 4)
            -0.5f, -0.5f,  0.5f,   1.0f, 0.0f, // 9  (duplicate of 0)
            -0.5f,  0.5f,  0.5f,   1.0f, 1.0f, // 10 (duplicate of 3)
            -0.5f,  0.5f, -0.5f,   0.0f, 1.0f, // 11 (duplicate of 7)
            // right face (x = +0.5)
             0.5f, -0.5f, -0.5f,   0.0f, 0.0f, // 12 (duplicate of 5)
             0.5f, -0.5f,  0.5f,   1.0f, 0.0f, // 13 (duplicate of 1)
             0.5f,  0.5f,  0.5f,   1.0f, 1.0f, // 14 (duplicate of 2)
             0.5f,  0.5f, -0.5f,   0.0f, 1.0f, // 15 (duplicate of 6)
            // top face (y = +0.5)
            -0.5f,  0.5f, -0.5f,   0.0f, 0.0f, // 16 (duplicate of 7)
             0.5f,  0.5f, -0.5f,   1.0f, 0.0f, // 17 (duplicate of 6)
             0.5f,  0.5f,  0.5f,   1.0f, 1.0f, // 18 (duplicate of 2)
            -0.5f,  0.5f,  0.5f,   0.0f, 1.0f, // 19 (duplicate of 3)
            // bottom face (y = -0.5)
            -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, // 20 (duplicate of 4)
             0.5f, -0.5f, -0.5f,   1.0f, 0.0f, // 21 (duplicate of 5)
             0.5f, -0.5f,  0.5f,   1.0f, 1.0f, // 22 (duplicate of 1)
            -0.5f, -0.5f,  0.5f,   0.0f, 1.0f, // 23 (duplicate of 0)
        };
        
        std::array indices = {
            // front face
            0, 1, 2,  2, 3, 0,
            // back face
            4, 6, 5,  6, 4, 7,
            // left face
            8, 9, 10, 10, 11, 8,
            // right face
            12, 14, 13, 14, 12, 15,
            // top face    (outward normal +y)
            16, 18, 17, 16, 19, 18,
            // bottom face (outward normal -y)
            20, 21, 22, 20, 22, 23
        };

        // Create and bind a Vertex Array Object (VAO)
        // In modern OpenGL, a VAO must be bound before setting vertex attributes.
        GL_CALL(glGenVertexArrays(1, &m_Graphics.VertexArray));
        GL_CALL(glBindVertexArray(m_Graphics.VertexArray));

        // Create and fill the Vertex Buffer Object (VBO)
        m_Graphics.VertexBuffer = SelectBuffer(GL_ARRAY_BUFFER, positions, GL_STATIC_DRAW);

        // Define the layout of the vertex data
        // Position attribute
        GL_CALL(glEnableVertexAttribArray(0));
        GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0));
        // UV attribute
        GL_CALL(glEnableVertexAttribArray(1));
        GL_CALL(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))));

        // Create and fill the Element Buffer Object (EBO)
        m_Graphics.IndexBuffer = SelectBuffer(GL_ELEMENT_ARRAY_BUFFER, indices, GL_STATIC_DRAW);

        // Load and compile shaders, then use the program
        ShaderSource source = ParseShader("res/shaders/cube.shader");
        m_Graphics.Shader = CreateShader(source.VertexSource, source.FragmentSource);

        // Load textures
        m_Graphics.Texture = CreateTexture("res/textures/grasstop.png");

        GL_CALL(glUseProgram(m_Graphics.Shader));

        // Fetch uniform locations
        m_Uniforms.ViewProjection = glGetUniformLocation(m_Graphics.Shader, "u_ViewProjection");
        m_Uniforms.Texture = glGetUniformLocation(m_Graphics.Shader, "u_Texture");

        // Upload texture to shader (0 = GL_TEXTURE0)
        GL_CALL(glUniform1i(m_Uniforms.Texture, 0));

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
        // Cube rotation (disabled for now)
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 mvp = projection * view * model;

        // Upload to shader
        GL_CALL(glUseProgram(m_Graphics.Shader));
        GL_CALL(glUniformMatrix4fv(m_Uniforms.ViewProjection, 1, GL_FALSE, glm::value_ptr(mvp)));
    }

    void UpdateCamera()
    {
        // Update rotation angles yaw and pitch
        glm::vec2 delta = m_Input.GetMouseDelta();
        m_Camera.Yaw = m_Camera.Yaw + delta.x * m_Camera.Sens;
        m_Camera.Pitch = glm::clamp(m_Camera.Pitch + delta.y * m_Camera.Sens, -89.0f, +89.0f);

        // Update front vector based on yaw and pitch
        glm::vec3 front;
        front.x = cos(glm::radians(m_Camera.Yaw)) * cos(glm::radians(m_Camera.Pitch));
        front.y = sin(glm::radians(m_Camera.Pitch));
        front.z = sin(glm::radians(m_Camera.Yaw)) * cos(glm::radians(m_Camera.Pitch));
        m_Camera.Dir = glm::normalize(front);

        // Update camera position
        float forwardInput = (float)(m_Input.IsKeyPressed(GLFW_KEY_W)) - (float)(m_Input.IsKeyPressed(GLFW_KEY_S));
        float sideInput = (float)(m_Input.IsKeyPressed(GLFW_KEY_D)) - (float)(m_Input.IsKeyPressed(GLFW_KEY_A));
        float upInput = (float)(m_Input.IsKeyPressed(GLFW_KEY_SPACE)) - (float)(m_Input.IsKeyPressed(GLFW_KEY_LEFT_SHIFT));
        glm::vec3 forward = m_Camera.Dir;
        glm::vec3 side = glm::normalize(glm::cross(m_Camera.Dir, m_Camera.Up));
        glm::vec3 up = m_Camera.Up;
        glm::vec3 movement = forwardInput * forward + sideInput * side + upInput * up;
        if (glm::length(movement) > 0.0f)
            m_Camera.Pos += glm::normalize(movement) * m_Camera.Speed * m_DeltaTime;
    }

    virtual void OnKeyboardEvent(int32_t key, int32_t action)
    {
        if (m_Input.IsKeyPressed(GLFW_KEY_ESCAPE))
            Close();
    }

    virtual void OnRender()
    {
        GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
        GL_CALL(glActiveTexture(GL_TEXTURE0));
        GL_CALL(glBindTexture(GL_TEXTURE_2D, m_Graphics.Texture.Index));
        GL_CALL(glBindVertexArray(m_Graphics.VertexArray));
        GL_CALL(glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr));
    }

    virtual void OnDestroy()
    {
        GL_CALL(glDeleteTextures(1, &m_Graphics.Texture.Index));
        GL_CALL(glDeleteProgram(m_Graphics.Shader));
        GL_CALL(glDeleteVertexArrays(1, &m_Graphics.VertexArray));
        GL_CALL(glDeleteBuffers(1, &m_Graphics.VertexBuffer));
        GL_CALL(glDeleteBuffers(1, &m_Graphics.IndexBuffer));
    }

private:
    Camera m_Camera;

    struct Cube
    {
        float Angle = 0.0f;
        glm::vec3 Axis = glm::vec3(0.5f, 1.0f, 0.3f);
        glm::vec3 Color = glm::vec3(0.2f, 0.2f, 0.2f);
    } m_Cube;

    struct Graphics
    {
        uint32_t Shader = 0;
        Texture Texture;
        uint32_t VertexArray = 0;
        uint32_t VertexBuffer = 0;
        uint32_t IndexBuffer = 0;
    } m_Graphics;

    struct Uniforms
    {
        int32_t ViewProjection = -1;
        int32_t Texture = -1;
    } m_Uniforms;
};

int main()
{
    Pseudocraft app{ "Pseudocraft", 1200, 800 };
    app.Start();
}
