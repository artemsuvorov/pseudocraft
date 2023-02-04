#include <iostream>
#include <array>

#include "core/log.h"
#include "core/application.h"

#include "graphics/graphics.h"
#include "graphics/shader.h"

class Pseudocraft : public Application
{    
private:
    uint32_t shader;

protected:
    virtual void OnInit()
    {
        std::array<float, 8> postions = {
            -0.5f, -0.5f, // 0
             0.5f, -0.5f, // 1
             0.5f,  0.5f, // 2
            -0.5f,  0.5f, // 3
        };
        
        std::array<uint32_t, 6> indices = {
            0, 1, 2,
            2, 3, 0
        };

        uint32_t buffer = SelectBuffer(GL_ARRAY_BUFFER, postions, GL_STATIC_DRAW);

        GL_CALL(glEnableVertexAttribArray(0));
        GL_CALL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0));

        uint32_t indexBufferObject = SelectBuffer(GL_ELEMENT_ARRAY_BUFFER, indices, GL_STATIC_DRAW);

        ShaderSource source = ParseShader("res/shaders/blue.shader");
        shader = CreateShader(source.VertexSource, source.FragmentSource);
        GL_CALL(glUseProgram(shader));
    }

    virtual void OnRender()
    {
        // GL_CALL(glDrawArrays(GL_TRIANGLES, 0, 6));
        GL_CALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
    }

    virtual void OnDestroy()
    {
        GL_CALL(glDeleteProgram(shader));
    }
};

int main()
{
    Pseudocraft app;
    app.Start();
    return 0;
}