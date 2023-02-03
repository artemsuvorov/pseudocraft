#include "shader.h"

void PrintCompilationErrors(uint32_t type, uint32_t compiledShaderId)
{
    int32_t result;
    glGetShaderiv(compiledShaderId, GL_COMPILE_STATUS, &result);

    if (result != GL_FALSE) return;
    
    int length;
    glGetShaderiv(compiledShaderId, GL_INFO_LOG_LENGTH, &length);
    char* message = (char*)alloca(length * sizeof(char));
    glGetShaderInfoLog(compiledShaderId, length, &length, message);
    std::cout << message << std::endl;
}

uint32_t CompileShader(uint32_t type, const std::string& src)
{
    uint32_t shaderId = glCreateShader(type);
    const char* srcPointer = src.c_str();

    glShaderSource(shaderId, 1, &srcPointer, nullptr);
    glCompileShader(shaderId);

    PrintCompilationErrors(type, shaderId);

    return shaderId;
}

uint32_t CreateShader(const std::string& vertexShaderSrc, const std::string& fragmentShaderSrc) 
{
    uint32_t program = glCreateProgram();

    uint32_t vertexShaderId = CompileShader(GL_VERTEX_SHADER, vertexShaderSrc);
    uint32_t fragmentShaderId = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSrc);
    
    glAttachShader(program, vertexShaderId);
    glAttachShader(program, fragmentShaderId);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderId);

    return program;
}