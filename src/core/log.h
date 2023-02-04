#pragma once

#include <iostream>
#include <sstream>
#include <iomanip>

#include <glad/glad.h>

#define ASSERT(errorcode) if ((errorcode) > 0) exit(errorcode);

#define GL_CALL(func) GlClearErrorStack();\
    func;\
    ASSERT(GlLogCall(#func, __FILE__, __LINE__))

void GlClearErrorStack();

uint32_t GlLogCall(const char* function, const char* file, uint32_t line);