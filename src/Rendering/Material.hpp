#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>
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
    std::shared_ptr<p6::Image>  texture;

};

struct Material {
    //struct MaterialParameters m_parameters {};

    std::vector<MaterialParameters> m_materials{};

    //std::shared_ptr<p6::Image>  m_texture; // p6::load_image("../assets/logo2.png")
    std::shared_ptr<p6::Shader> m_shader;  // p6::load_shader("../src/shaders/3D.vs.glsl", "../src/shaders/light.fs.glsl")

    Material() = default;

    // Material(MaterialParameters params, std::shared_ptr<p6::Shader> shader, std::shared_ptr<p6::Image> texture)
    //     : m_parameters(params), m_shader(shader), m_texture(texture){};

    Material(std::vector<tinyobj::material_t>& materials) {
        for(auto const& material : materials) {
            m_materials.push_back(MaterialParameters{.diffuse = glm::vec3(material.diffuse[0], material.diffuse[1], material.diffuse[2]), .reflexion = glm::vec3(material.specular[0], material.specular[1], material.specular[2]), .glossy = glm::vec3(0.5), .shininess = material.shininess, .alpha = 1., .texture = std::make_shared<p6::Image, p6::Image>(p6::load_image(material.diffuse_texname))});
        }
    }
};
