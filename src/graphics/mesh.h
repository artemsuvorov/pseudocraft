#pragma once

#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float2.hpp>
#include <vector>


struct Vertex
{
    glm::vec3 Pos = glm::vec3(0.0f);
    glm::vec2 Uv = glm::vec2(0.0f);
};


struct Mesh
{
    std::vector<Vertex> Vertices;
    std::vector<uint32_t> Indices;
};


struct GraphicsMesh
{
    uint32_t VertexArray = 0;
    uint32_t VertexBuffer = 0;
    uint32_t IndexBuffer = 0;
};

void GenerateCube(Mesh& mesh);
GraphicsMesh CreateMesh(const Mesh& mesh);
