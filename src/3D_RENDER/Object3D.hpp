#pragma once

#include "Mesh.hpp"
#include "Material.hpp"
#include "glm/gtx/transform.hpp"

struct Object3D {
    std::shared_ptr<Mesh> m_mesh;
    std::shared_ptr<Material> m_material;
};
