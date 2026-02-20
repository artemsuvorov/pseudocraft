#include "log.h"

std::string hex(uint32_t number)
{
    const uint32_t WIDTH = 4;
    std::ostringstream output;
    output << "0x" << std::setw(WIDTH) << std::setfill('0') << std::hex << number;
    return output.str();
}

void GlClearErrorStack()
{
    while (glGetError() != GL_NO_ERROR);
}

uint32_t GlLogCall(const char* function, const char* file, uint32_t line)
{
    GLenum errorcode;
    while ((errorcode = glGetError()) != GL_NO_ERROR)
    {
        std::cout << "[OpenGL ERROR] (" << hex(errorcode) << "): " 
            << function << " " << file << ":" << line << std::endl;
        return errorcode;
    }

    return GL_NO_ERROR;
}