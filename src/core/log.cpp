#include "log.h"

void GlClearErrorStack()
{
    while (glGetError() != GL_NO_ERROR);
}

uint32_t GlLogCall(const char* function, const char* file, uint32_t line)
{
    GLenum errorcode;
    while ((errorcode = glGetError()) != GL_NO_ERROR)
    {
        std::cout << "[OpenGL ERROR] (" << errorcode << "): " <<
            function << " " << file << ":" << line << std::endl;
        return errorcode;
    }

    return GL_NO_ERROR;
}