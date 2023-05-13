#pragma once

#include "Material.hpp"
#include "Mesh.hpp"
#include "glm/gtx/transform.hpp"


struct Object3D {
    std::shared_ptr<Mesh> m_mesh;
    Material              m_material;

};
