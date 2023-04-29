#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <utility>
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/scalar_constants.hpp"
#include "glm/fwd.hpp"

#include "p6/p6.h"

#include "glimac/tiny_obj_loader.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
struct Material {
    glm::vec3 diffuse{};
    glm::vec3 reflexion{};
    glm::vec3 glossy{};
    float     shininess{};

    // TODO
    // int texture;

    p6::Shader shader = p6::load_shader("../src/3D_RENDER/shaders/3D.vs.glsl", "../src/3D_RENDER/shaders/light.fs.glsl");

    Material(glm::vec3 diffuse, glm::vec3 reflexion, glm::vec3 glossy, float shininess): 
        diffuse(diffuse), reflexion(reflexion), glossy(glossy), shininess(shininess){};

    Material()

};

