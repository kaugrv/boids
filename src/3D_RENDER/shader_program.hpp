#pragma once

#include <vcruntime.h>
#include <vcruntime_typeinfo.h>
#include <algorithm>
#include <array>
#include <cmath>
#include <utility>
#include <vector>
#include "3D_RENDER/Material.hpp"
#include "3D_RENDER/shader_program.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/scalar_constants.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/random.hpp"
#include "light.hpp"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <filesystem>
#include <string_view>
#include <typeinfo>
#include "3D_RENDER/light.hpp"
#include "p6/p6.h"

struct lightDatas {
    std::vector<glm::vec3> position_or_direction_view{};
    std::vector<glm::vec3> list_color{};
    std::vector<float>     list_intensity{};
};

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

template<typename T> // works for point and directionnal light
void send_light(p6::Shader& shader, const std::vector<T>& list_light, const glm::mat4& view_mat, const bool& is_directionnal)
{
    lightDatas lights_datas = fill_light_data(list_light, view_mat);
    if (is_directionnal)
    {
        send_light_dir_uniform(shader, lights_datas);
        send_light_pos_uniform(shader, lights_datas);
    }
}
void set_lights(p6::Shader& shader, const std::vector<PointLight>& list_light, const std::vector<DirectionalLight>& list_directionnal_light, const glm::mat4& view_mat)
{
    send_light<PointLight>(shader, list_light, view_mat, false);
    send_light<DirectionalLight>(shader, list_directionnal_light, view_mat, true);
}

void send_light_pos_uniform(p6::Shader& shader, const lightDatas& light_datas)
{
    GLint  shader_id = shader.id();
    size_t nb_light  = light_datas.list_color.size();

    GLuint uIncidentLight  = glad_glGetUniformLocation(shader_id, "uIncidentLight");
    GLuint uLightColor     = glad_glGetUniformLocation(shader_id, "uLightColor");
    GLuint uLightIntensity = glad_glGetUniformLocation(shader_id, "uLightIntensity");

    // actually set them
    glUniform3fv(uIncidentLight, nb_light, (const GLfloat*)light_datas.position_or_direction_view.data());
    glUniform3fv(uLightColor, nb_light, (const GLfloat*)light_datas.list_color.data());
    glUniform1fv(uLightIntensity, nb_light, (const GLfloat*)light_datas.list_intensity.data());
    shader.set("u_nb_light", nb_light);
}

void send_light_dir_uniform(p6::Shader& shader, lightDatas light_datas)
{
    GLint  shader_id = shader.id();
    size_t nb_light  = light_datas.list_color.size();

    GLuint uDirectionLight          = glad_glGetUniformLocation(shader_id, "uDirectionLight");
    GLuint uLightColor              = glad_glGetUniformLocation(shader_id, "uLightColor");
    GLuint uDirectionLightIntensity = glad_glGetUniformLocation(shader_id, "uDirectionLightIntensity");

    // actually set them
    glUniform3fv(uDirectionLight, nb_light, (const GLfloat*)light_datas.position_or_direction_view.data());
    glUniform3fv(uLightColor, nb_light, (const GLfloat*)light_datas.list_color.data());
    glUniform1fv(uDirectionLightIntensity, nb_light, (const GLfloat*)light_datas.list_intensity.data());
    shader.set("u_nb_light", nb_light);
}

glm::vec3 get_position_or_direction(PointLight light)
{
    return light.position;
}

glm::vec3 get_position_or_direction(DirectionalLight light)
{
    return light.direction;
}

template<typename T>
lightDatas fill_light_data(const std::vector<T>& list_light, const glm::mat4& view_mat)
{
    lightDatas lights_datas;
    for (auto const& light : list_light)
    {
        glm::vec3 pos_or_dir      = get_position_or_direction(light);
        glm::vec3 pos_or_dir_view = transform_in_view_space(pos_or_dir, view_mat, 1); // in view space

        lights_datas.position_or_direction_view.push_back(pos_or_dir_view);
        lights_datas.list_color.push_back(light.color);
        lights_datas.list_intensity.push_back(light.intensity);
    }
    return lights_datas;
}

void set_blinn_phong(p6::Shader& shader, Material& material, const std::vector<PointLight>& list_light, const std::vector<DirectionalLight>& list_directionnal_light, const glm::mat4& MV, const glm::mat4& ProjMatrix)
{
    set_matrix(shader, MV, ProjMatrix);
    set_material(shader, material);
    set_lights(shader, list_light, list_directionnal_light, MV);
}