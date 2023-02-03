#include <iostream>
#include <array>

#include "core/application.h"
#include "graphics/shader.h"

class Pseudocraft : public Application
{
private:
    uint32_t shader;

protected:
    virtual void OnInit()
    {
        std::array<float, 6> postions = 
        {
            -0.5f, -0.5f,
            0.0f,  0.5f,
            0.5f, -0.5f
        };
        uint32_t size = 6 * sizeof(float);

        uint32_t buffer;
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, size, &postions, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        ShaderSource source = ParseShader("res/shaders/blue.shader");
        shader = CreateShader(source.VertexSource, source.FragmentSource);
        glUseProgram(shader);
    }

    virtual void OnRender()
    {
        glDrawArrays(GL_TRIANGLES, 0, 3);   
    }

    virtual void OnDestroy()
    {
        glDeleteProgram(shader);
    }
};

int main()
{
    Pseudocraft app;
    app.Start();
    return 0;
}