#include <iostream>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "graphics.h"
#include "mesh.h"
#include "../core/log.h"


void GenerateCube(Mesh& mesh)
{
    mesh.Vertices.clear();
    mesh.Indices.clear();
    mesh.Vertices.reserve(24);
    mesh.Indices.reserve(36);

    struct Face final
    {
        glm::vec3 normal;
        glm::vec3 tangent;
        glm::vec3 bitangent;
    };

    constexpr float size = 1.0f;
    constexpr float h = size * 0.5f;
    constexpr Face faces[6] = {
        { { 0, 0, 1}, {1, 0, 0}, {0, 1, 0} },  // front
        { { 0, 0,-1}, {1, 0, 0}, {0,-1, 0} },  // back
        { {-1, 0, 0}, {0, 1, 0}, {0, 0,-1} },  // left
        { { 1, 0, 0}, {0, 1, 0}, {0, 0, 1} },  // right
        { { 0, 1, 0}, {0, 0, 1}, {1, 0, 0} },  // top
        { { 0,-1, 0}, {0, 0, 1}, {-1,0, 0} },  // bottom
    };

    for (int32_t index = 0; index < 6; index++)
    {
        const Face& face = faces[index];
        const uint32_t base = mesh.Vertices.size();
        const glm::vec2 uvs[4] = { {-1, -1}, {+1, -1}, {+1,  1}, {-1, +1}, };

        for (const glm::vec2 uv : uvs)
        {
            Vertex& vertex = mesh.Vertices.emplace_back();
            vertex.Pos = face.normal * h + uv.x * h * face.tangent + uv.y * h * face.bitangent;
            vertex.Uv = (uv + 1.0f) * 0.5f;
        }

        mesh.Indices.insert(mesh.Indices.end(), {
            base + 0, base + 1, base + 2,
            base + 2, base + 3, base + 0,
        });
    }
}

GraphicsMesh CreateMesh(const Mesh& mesh)
{
    GraphicsMesh gfx;

    // Create and bind VAO
    glGenVertexArrays(1, &gfx.VertexArray);
    glBindVertexArray(gfx.VertexArray);

    // Create and fill VBO
    glGenBuffers(1, &gfx.VertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, gfx.VertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, mesh.Vertices.size() * sizeof(Vertex), mesh.Vertices.data(), GL_STATIC_DRAW);

    // Set up attributes
    // Position (offset 0)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Pos));
    // UV (offset after position)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Uv));

    // Create and fill EBO
    glGenBuffers(1, &gfx.IndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gfx.IndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.Indices.size() * sizeof(uint32_t), mesh.Indices.data(), GL_STATIC_DRAW);

    // Unbind VAO (optional)
    glBindVertexArray(0);

    return gfx;
}
