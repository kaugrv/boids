#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <utility>
#include <vector>
#include "3D_RENDER/Material.hpp"
#include "glimac/Image.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/scalar_constants.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/random.hpp"
#include "light.hpp"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <filesystem>
#include <glimac/FilePath.hpp>
#include <glimac/Program.hpp>
#include <glimac/Sphere.hpp>
#include <glimac/glm.hpp>
#include "Material.hpp"
#include "light.hpp"
#include "p6/p6.h"

namespace fs = std::filesystem;

glm::vec3 transform_in_view_space(glm::vec3 vector, glm::mat4 view_matrix, int is_point)
{
    glm::vec4 h_vector{vector.x, vector.y, vector.z, is_point};
    glm::vec4 h_vector_view = view_matrix * h_vector;
    glm::vec3 vector_view{h_vector_view.x, h_vector_view.y, h_vector_view.z};
    return vector_view;
}

void set_matrix(p6::Shader& shader, const glm::mat4& MV, const glm::mat4& ProjMatrix)
{
    shader.set("uMVMatrix", MV);
    shader.set("uNormalMatrix", glm::transpose(glm::inverse(MV)));
    shader.set("uMVPMatrix", ProjMatrix * MV);
}

void set_material(p6::Shader& shader, const Material& material)
{
    shader.set("uDiffuse", material.diffuse);
    shader.set("uGlossy", material.glossy);
    shader.set("uShininess", material.shininess);
}

void set_lights(p6::Shader& shader, const std::vector<PointLight>& list_light, const std::vector<DirectionalLight>& list_directionnal_light, const glm::mat4& view_mat)
{
    std::vector<glm::vec3> list_pos_view;
    std::vector<glm::vec3> list_color;
    std::vector<float>     list_intensity;

    std::vector<glm::vec3> list_direction_view;
    std::vector<glm::vec3> list_color_directionnal;
    std::vector<float>     list_intensity_directionnal;

    // setup data list directional light
    for (auto const& light : list_light)
    {
        glm::vec3 light_pos_view = transform_in_view_space(light.position, view_mat, 1); // in view space
        list_pos_view.push_back(light_pos_view);
        list_color.push_back(light.color);
        list_intensity.push_back(light.intensity);
    }
    int nb_light = list_light.size();

    // setup data list directional light
    for (auto const& dir_light : list_directionnal_light)
    {
        glm::vec3 light_dir_view = transform_in_view_space(dir_light.direction, view_mat, 0); // in view space
        list_direction_view.push_back(light_dir_view);
        list_color_directionnal.push_back(dir_light.color);
        list_intensity_directionnal.push_back(dir_light.intensity);
    }
    int nb_dir_light = list_directionnal_light.size();

    // actually set them
    // glUniform3fv(uIncidentLight, nb_light, (const GLfloat*)list_pos_view.data());
    shader.set("uIncidentLight", list_pos_view.data());
    // glUniform3fv(uLightColor, nb_light, (const GLfloat*)list_color.data());
    shader.set("uLightColor", list_color.data());
    // glUniform1fv(uLightIntensity, nb_light, (const GLfloat*)list_intensity.data());
    shader.set("uLightIntensity", list_intensity.data());
    shader.set("u_nb_light", nb_light);

    // glUniform3fv(uDirectionLight, nb_dir_light, (const GLfloat*)list_direction_view.data());
    shader.set("uDirectionLight", list_direction_view.data());
    // glUniform3fv(uDirectionLightColor, nb_dir_light, (const GLfloat*)list_color_directionnal.data());
    shader.set("uDirectionLightColor", list_color_directionnal.data());
    // glUniform1fv(uDirectionLightIntensity, nb_dir_light, (const GLfloat*)list_intensity_directionnal.data());
    shader.set("uDirectionLightIntensity", list_intensity_directionnal.data());
    shader.set("u_direction_nb_light", nb_dir_light);
}

void set_blinn_phong(p6::Shader& shader, Material& material, const std::vector<PointLight>& list_light, const std::vector<DirectionalLight>& list_directionnal_light, const glm::mat4& MV, const glm::mat4& ProjMatrix)
{
    set_matrix(shader, MV, ProjMatrix);
    set_material(shader, material);
    set_lights(shader, list_light, list_directionnal_light, MV);
}