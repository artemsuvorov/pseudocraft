#pragma once

#include <iostream>
#include <array>

#include <glad/glad.h>

#include "../core/log.h"

/// @brief Creates and initializes a new data store for a buffer object.
/// @tparam Type Type of the provided data.
/// @tparam Size Number of elements within the provided data.
/// @param type Specifies the target to which the buffer object 
/// is bound for glBufferData (GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER, etc.)
/// @param data Specifies a pointer to data that will be copied into 
/// the data store for initialization, or NULL if no data is to be copied.
/// @param usage Specifies the expected usage pattern of the data store
/// (GL_STREAM_DRAW, GL_DYNAMIC_READ, etc.) 
/// @return The id to the generated buffer.
template <typename Type, size_t Size> 
uint32_t SelectBuffer(uint32_t type, std::array<Type, Size> data, uint32_t usage);

template <typename Type, size_t Size>
inline uint32_t SelectBuffer(uint32_t type, std::array<Type, Size> data, uint32_t usage)
{
    uint32_t buffer;
    GL_CALL(glGenBuffers(1, &buffer));
    GL_CALL(glBindBuffer(type, buffer));
    GL_CALL(glBufferData(type, Size * sizeof(Type), &data, usage));
    return buffer;
}
