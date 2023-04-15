#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <utility>
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/scalar_constants.hpp"
#include "glm/fwd.hpp"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glimac/glm.hpp>

struct PointLight {
    glm::vec3 position;
    glm::vec3 color;

    float intensity;

    friend std::ostream& operator<<(std::ostream& os, const PointLight& cam);
};

struct DirectionalLight {
    glm::vec3 direction;
    glm::vec3 color;
    float     intensity;
};

inline std::ostream& operator<<(std::ostream& os, const PointLight& col)
{
    os << "color : " << col.color << std::endl;
    os << "pos : " << col.position;
    return os;
}