#pragma once

#include "glimac/common.hpp"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "glimac/Sphere.hpp"
#include "glimac/Cone.hpp"
#include "p6/p6.h"

void fill_vbo(GLuint& m_vbo, const int& vertex_count, const glimac::ShapeVertex* shape_data_pointer)
{
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof(glimac::ShapeVertex), shape_data_pointer, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void setup_vao(GLuint& m_vao, const GLuint& m_vbo)
{
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    const GLuint VERTEX_ATTR_POSITION  = 0;
    const GLuint NORMAL_ATTR_POSITION  = 1;
    const GLuint TEXTURE_ATTR_POSITION = 2;

    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(NORMAL_ATTR_POSITION);
    glEnableVertexAttribArray(TEXTURE_ATTR_POSITION);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)offsetof(glimac::ShapeVertex, position));
    glVertexAttribPointer(NORMAL_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)offsetof(glimac::ShapeVertex, normal));
    glVertexAttribPointer(TEXTURE_ATTR_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)offsetof(glimac::ShapeVertex, texCoords));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

class Mesh {
private:
    GLuint  m_vbo;
    GLuint  m_vao;
    GLsizei vertex_count;

public:
    Mesh()                 = default;
    Mesh(const Mesh& mesh) = default;

    ~Mesh()
    {
        glDeleteBuffers(1, &m_vbo);
        glDeleteVertexArrays(1, &m_vao);
    };

    // TODO constructor from a file path (.obj)
    Mesh(const glimac::Sphere& sphr)
        : vertex_count(sphr.getVertexCount())
    {
        fill_vbo(m_vbo,sphr.getVertexCount(), sphr.getDataPointer());
        setup_vao(m_vao, m_vbo);
    }

    Mesh(const glimac::Cone& cone)
        : vertex_count(cone.getVertexCount())
    {
        fill_vbo(m_vbo, cone.getVertexCount(), cone.getDataPointer());
        setup_vao(m_vao, m_vbo);
    }

    GLuint get_vao()
    {
        return m_vao;
    }
    GLuint get_vbo()
    {
        return m_vbo;
    }
    GLsizei get_vertex_count()
    {
        return vertex_count;
    }
};

void drawMesh(Mesh &mesh) {
    glBindVertexArray(mesh.get_vao()); // Bind Mesh
    glDrawArrays(GL_TRIANGLES, 0, mesh.get_vertex_count());
}