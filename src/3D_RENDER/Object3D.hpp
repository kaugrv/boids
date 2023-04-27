#pragma once

#include "Mesh.hpp"
#include "Material.hpp"
#include "glm/gtx/transform.hpp"

struct Object3D {
    Mesh m_mesh{};
    Material* m_material{};
};
