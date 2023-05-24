#pragma once

#include "Light.hpp"
#include "Material.hpp"
#include "Mesh.hpp"
#include "glm/gtx/transform.hpp"
#include "shader_program.hpp"

struct Object3D {
    std::shared_ptr<Mesh> m_mesh;
    Material              m_material;

    void drawObject(const glm::mat4& ViewMatrix, const glm::mat4& ModelMatrix, const glm::mat4& ProjMatrix, const std::vector<PointLight>& list_light, const std::vector<DirectionalLight>& list_directional_light, const ShadowMap& shadowMap)
    {
        m_material.m_shader->use();
        set_blinn_phong(m_material, list_light, list_directional_light, ViewMatrix, ModelMatrix, ProjMatrix, shadowMap);
        drawMesh(*m_mesh);
    }

    void drawObjectFromLightView(const glm::mat4& LightViewMatrix, const glm::mat4& ModelMatrix, const glm::mat4& ProjMatrix) const
    {
        // Use the object's shader program
        m_material.m_shader->use();

        // Set the light space matrix uniform
        glm::mat4 lightSpaceMatrix = LightViewMatrix * ModelMatrix;
        (m_material.m_shader)->set("lightSpaceMatrix", lightSpaceMatrix);

        // Draw the object using the object's shader
        drawMesh(*m_mesh);
    }
};
