#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <memory>
#include <utility>
#include "glimac/tiny_obj_loader.h"
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
    struct MaterialParameters m_parameters {};

    std::shared_ptr<p6::Image>  m_texture; // p6::load_image("../assets/logo2.png")
    std::shared_ptr<p6::Shader> m_shader;  // p6::load_shader("../src/shaders/3D.vs.glsl", "../src/shaders/light.fs.glsl")

    Material() = default;

    Material(MaterialParameters params, std::shared_ptr<p6::Shader> shader, std::shared_ptr<p6::Image> texture)
        : m_parameters(params), m_shader(shader), m_texture(texture){};
};
