#pragma once

#include <iostream>
#include <glad/glad.h>

/*
 * Compile the shaders provided as strings to a single OpenGL shader program 
 * and return its unique ID as an integer.
 */
uint32_t CreateShader(const std::string& vertexShaderSrc, const std::string& fragmentShaderSrc);