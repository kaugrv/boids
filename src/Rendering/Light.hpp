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
    float     intensity;

    void updatae_light_position(const glm::vec3& surveyor_position) {
        position = surveyor_position;
        position.z +=0.1;

    }
};

struct DirectionalLight {
    glm::vec3 direction;
    glm::vec3 color;
    float     intensity;
};


