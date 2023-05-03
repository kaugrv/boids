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

std::ostream& operator<<(std::ostream& os, glm::mat4 mat4)
{
    glm::mat4 mat4_2 = mat4;
    os << "[" << mat4_2[0][0] << " , " << mat4_2[1][0] << " , " << mat4_2[2][0] << " , " << mat4_2[3][0] << "]"
       << "\n";
    os << "[" << mat4_2[0][1] << " , " << mat4_2[1][1] << " , " << mat4_2[2][1] << " , " << mat4_2[3][1] << "]"
       << "\n";
    os << "[" << mat4_2[0][2] << " , " << mat4_2[1][2] << " , " << mat4_2[2][2] << " , " << mat4_2[3][2] << "]"
       << "\n";
    os << "[" << mat4_2[0][3] << " , " << mat4_2[1][3] << " , " << mat4_2[2][3] << " , " << mat4_2[3][3] << "]"
       << "\n";
    return os;
}