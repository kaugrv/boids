#pragma once

#include "Material.hpp"
#include "Mesh.hpp"
#include "Rendering/Material.hpp"
#include "glm/gtx/transform.hpp"

struct Object3D {
    std::shared_ptr<Mesh>      m_mesh;
    std::shared_ptr<Material>  m_material;
    std::shared_ptr<Materials> m_materials;
};
