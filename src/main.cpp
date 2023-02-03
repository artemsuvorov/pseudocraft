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

        std::string vertrexShader =
            "#version 330 core\n"
            "\n"
            "layout(location = 0) in vec4 position;\n"
            "\n"
            "void main()\n"
            "{\n"
            "   gl_Position = position;\n"
            "}\n";

        std::string fragmentShader =
            "#version 330 core\n"
            "\n"
            "layout(location = 0) out vec4 color;\n"
            "\n"
            "void main()\n"
            "{\n"
            "   color = vec4(0.0, 1.0, 0.0, 1.0);\n"
            "}\n";

        shader = CreateShader(vertrexShader, fragmentShader);
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