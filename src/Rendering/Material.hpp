#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <utility>
#include "../../lib/tinyobjloader/tiny_obj_loader.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/scalar_constants.hpp"
#include "glm/fwd.hpp"
#include "p6/p6.h"


#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

struct MaterialParameters {
    glm::vec3 diffuse{};
    glm::vec3 reflexion{};
    glm::vec3 glossy{};
    float     shininess{};
    float     alpha{};
};

struct Material {
    struct MaterialParameters parameters {};

    p6::Image  texture = p6::load_image("../assets/logo2.png");
    p6::Shader shader  = p6::load_shader("../src/shaders/3D.vs.glsl", "../src/shaders/light.fs.glsl");

    Material() = default;

    Material(glm::vec3 diffuse, glm::vec3 reflexion, glm::vec3 glossy, float shininess, float alpha)
        : parameters{diffuse, reflexion, glossy, shininess, alpha} {};

    Material(const tinyobj::material_t& material)
        : parameters{glm::vec3(material.diffuse[0], material.diffuse[1], material.diffuse[2]), glm::vec3(material.specular[0], material.specular[1], material.specular[2]), glm::vec3(material.ambient[0], material.ambient[1], material.ambient[2]), material.shininess} {};
};
