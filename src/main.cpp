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
        std::array<float, 8> postions = 
        {
            -0.5f, -0.5f,
             0.5f, -0.5f,
             0.5f,  0.5f,

            //  0.5f,  0.5f,
            -0.5f,  0.5f,
            // -0.5f, -0.5f,
        };
        uint32_t positionSize = 8 * sizeof(float);
        
        std::array<uint32_t, 6> indices =
        {
            0, 1, 2,
            2, 3, 0
        };
        uint32_t indexSize = 6 * sizeof(uint32_t);

        uint32_t buffer;
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), &postions, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

        uint32_t indexBufferObject;
        glGenBuffers(1, &indexBufferObject);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize, &indices, GL_STATIC_DRAW);

        // glBindBuffer(GL_ARRAY_BUFFER, 0);

        ShaderSource source = ParseShader("res/shaders/blue.shader");
        shader = CreateShader(source.VertexSource, source.FragmentSource);
        glUseProgram(shader);
    }

    virtual void OnRender()
    {
        // glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
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