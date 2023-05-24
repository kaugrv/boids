#pragma once

#include "Rendering/Light.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "p6/p6.h"

class ShadowMap {
public:
    GLuint  m_depth_texture;
    GLuint  m_frame_buffer_id;
    GLsizei m_shadow_map_resolution;

    ShadowMap(p6::Context& ctx);

    glm::mat4 getLightSpaceMatrix(const PointLight& light, const glm::mat4& ModelMatrix, const glm::mat4& ProjMatrix)
        const
    {
        // Define the light's position
        glm::vec3 lightPosition = light.position;

        // Extract the translation component from the ModelMatrix
        glm::vec3 targetPosition = glm::vec3(ModelMatrix[3]);

        // Define the up direction
        glm::vec3 upDirection = glm::vec3(0., 1., 0.);

        // Calculate the light's view matrix
        glm::mat4 LightViewMatrix = glm::lookAt(lightPosition, targetPosition, upDirection);

        // Calculate the light space matrix
        glm::mat4 LightSpaceMatrix = ProjMatrix * LightViewMatrix * ModelMatrix;

        return LightSpaceMatrix;
    }
};

ShadowMap::ShadowMap(p6::Context& ctx)
{
    // Create depth texture
    glGenTextures(1, &m_depth_texture);
    glBindTexture(GL_TEXTURE_2D, m_depth_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, ctx.main_canvas_width(), ctx.main_canvas_height(), 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Create frame buffer
    glGenFramebuffers(1, &m_frame_buffer_id);
    glBindFramebuffer(GL_FRAMEBUFFER, m_frame_buffer_id);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depth_texture, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}