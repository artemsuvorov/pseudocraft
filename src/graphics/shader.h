#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <glad/glad.h>

struct ShaderSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

/*
 * Parses vertex shader and a fragment shader from a file. 
 */
ShaderSource ParseShader(const std::string& filepath);

/*
 * Compile the shaders provided as strings to a single OpenGL shader program 
 * and return its unique ID as an integer.
 */
uint32_t CreateShader(const std::string& vertexShaderSrc, const std::string& fragmentShaderSrc);