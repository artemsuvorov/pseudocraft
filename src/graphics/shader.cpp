#include "shader.h"

ShaderSource ParseShader(const std::string& filepath)
{
    std::ifstream file(filepath);
    std::stringstream shaderSrc[2];

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    ShaderType type = ShaderType::NONE;
    
    while (getline(file, line))
    {
        if (line.find("#shader") == std::string::npos) 
            shaderSrc[(int)type] << line << '\n';
        else if (line.find("vertex") != std::string::npos)
            type = ShaderType::VERTEX;
        else if (line.find("fragment") != std::string::npos)
            type = ShaderType::FRAGMENT;
    }

    return { shaderSrc[0].str(), shaderSrc[1].str() };
}

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

uint32_t CreateShader(const std::string &vertexShaderSrc, const std::string &fragmentShaderSrc)
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