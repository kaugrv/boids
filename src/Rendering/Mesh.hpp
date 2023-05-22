#pragma once

#include <vector>
#include "glimac/common.hpp"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "glimac/tiny_obj_loader.h"
#include "p6/p6.h"


void fill_vbo_obj(GLuint& m_vbo, const std::vector<tinyobj::shape_t>& shapes)
{
    std::vector<GLfloat> vertices{};

    for (const auto& shape : shapes)
    {
        const auto& positions = shape.mesh.positions;
        const auto& normals   = shape.mesh.normals;
        const auto& texcoords = shape.mesh.texcoords;

        for (size_t i = 0; i < positions.size(); i += 3)
        {
            vertices.push_back(positions[i]);
            vertices.push_back(positions[i + 1]);
            vertices.push_back(positions[i + 2]);

            vertices.push_back(normals[i]);
            vertices.push_back(normals[i + 1]);
            vertices.push_back(normals[i + 2]);

            vertices.push_back(texcoords[i / 3]);
            vertices.push_back(texcoords[i / 3 + 1]);
        }
    }

    // Create the VBO and fill it with the vertex data
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void fill_ibo_obj(GLuint& m_ibo, const std::vector<tinyobj::shape_t>& shapes)
{
    std::vector<unsigned int> indices;

    for (int n = 0; n < shapes.size(); n++)
    {
        const auto& shape_indices = shapes[n].mesh.indices;
        indices.insert(indices.end(), shape_indices.begin(), shape_indices.end());
    }

    glGenBuffers(1, &m_ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
}

void setup_vao_obj(GLuint& m_vao, const GLuint& m_vbo, GLuint& m_ibo, const int& vertex_count)
{
    // Generate a new VAO and bind it
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    // Enable the attribute arrays
    glEnableVertexAttribArray(0); // position
    glEnableVertexAttribArray(1); // normal
    glEnableVertexAttribArray(2); // texCoords

    // Bind the VBO and specify the attribute pointers
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);                     // position
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat))); // normal
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat))); // texCoords

    // Bind the IBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);

    // Unbind the VBO and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

GLsizei get_vertex_count_obj(const std::vector<tinyobj::shape_t>& shapes)
{
    GLsizei n = 0;
    for (const auto& shape : shapes)
    {
        n += shape.mesh.indices.size();
    }

    return n;
}

class Mesh {
private:
    GLuint  m_vbo;
    GLuint  m_vao;
    GLuint  m_ibo;
    GLsizei vertex_count = 0;

public:
    Mesh()                 = default;
    Mesh(const Mesh& mesh) = default;

    ~Mesh(){
        // glDeleteBuffers(1, &m_vbo);
        // glDeleteVertexArrays(1, &m_vao);
    };

    Mesh(const std::vector<tinyobj::shape_t>& shapes)
        : vertex_count(get_vertex_count_obj(shapes))
    {
        fill_vbo_obj(m_vbo, shapes);
        fill_ibo_obj(m_ibo, shapes);
        setup_vao_obj(m_vao, m_vbo, m_ibo, get_vertex_count_obj(shapes));
    }

    GLuint get_vao()
    {
        return m_vao;
    }
    GLuint get_vbo()
    {
        return m_vbo;
    }
    GLuint get_ibo()
    {
        return m_ibo;
    }
    GLsizei get_vertex_count()
    {
        return vertex_count;
    }
};

void drawMesh(Mesh& mesh)
{
    glBindVertexArray(mesh.get_vao());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.get_ibo());
    glDrawElements(GL_TRIANGLES, mesh.get_vertex_count(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}