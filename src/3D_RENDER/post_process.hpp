#pragma once
#include "p6/p6.h"

class QuadPostProcess {
public:
    GLuint  m_vbo;
    GLuint  m_vao;
    GLsizei vertex_count = 6;

    QuadPostProcess();
};
QuadPostProcess::QuadPostProcess()
{
    float quadVertices[] = {// vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
                            // positions   // texCoords
                            -1.0f, 1.0f, 0.0f, 1.0f,
                            -1.0f, -1.0f, 0.0f, 0.0f,
                            1.0f, -1.0f, 1.0f, 0.0f,

                            -1.0f, 1.0f, 0.0f, 1.0f,
                            1.0f, -1.0f, 1.0f, 0.0f,
                            1.0f, 1.0f, 1.0f, 1.0f};

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}

class FrameBuffer {
public:
    GLuint m_frame_buffer_id;
    GLuint m_texture_object;
    GLuint m_render_object;
    GLuint m_depth_texture;

    FrameBuffer(p6::Context& ctx);
};
FrameBuffer::FrameBuffer(p6::Context& ctx)
{
    // --- setup color texture
    glGenTextures(1, &m_texture_object);
    glBindTexture(GL_TEXTURE_2D, m_texture_object);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ctx.main_canvas_width(), ctx.main_canvas_height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    // --- setup depth texture
    glGenTextures(1, &m_depth_texture);
    glBindTexture(GL_TEXTURE_2D, m_depth_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, ctx.main_canvas_width(), ctx.main_canvas_height(), 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    // --- setup framebuffer
    glGenFramebuffers(1, &m_frame_buffer_id);
    glBindFramebuffer(GL_FRAMEBUFFER, m_frame_buffer_id);
    // glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depth_texture, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture_object, 0);

    // --- setup renderbuffer
    glGenRenderbuffers(1, &m_render_object);
    glBindRenderbuffer(GL_RENDERBUFFER, m_render_object);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, ctx.main_canvas_width(), ctx.main_canvas_height());
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_render_object);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

class PostProcess {
public:
    p6::Shader      m_shader;
    QuadPostProcess m_quad;
    FrameBuffer     m_frame_buffer;

    PostProcess(p6::Context& ctx)
        : m_shader(p6::load_shader("../src/3D_RENDER/shaders/post_process.vs.glsl", "../src/3D_RENDER/shaders/post_process.fs.glsl")), m_quad(), m_frame_buffer(ctx){};
};
