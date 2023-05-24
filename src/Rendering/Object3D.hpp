#pragma once

#include "Light.hpp"
#include "Material.hpp"
#include "Mesh.hpp"
#include "glm/gtx/transform.hpp"
#include "shader_program.hpp"

struct Object3D {
    std::shared_ptr<Mesh> m_mesh;
    Material              m_material;

    void drawObject(const glm::mat4& ViewMatrix, const glm::mat4& ModelMatrix, const glm::mat4& ProjMatrix, const std::vector<PointLight>& list_light, const std::vector<DirectionalLight>& list_directionnal_light)
    {
        m_material.m_shader->use();
        set_blinn_phong(m_material, list_light, list_directionnal_light, ViewMatrix, ModelMatrix, ProjMatrix);
        drawMesh(*m_mesh);
    }
};
