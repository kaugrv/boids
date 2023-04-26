#pragma once

#include <ostream>
#include <vector>
#include "3D_RENDER/Material.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/scalar_constants.hpp"
#include "glm/fwd.hpp"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
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
    shader.set("uMVPMatrix", ProjMatrix * MV);
    shader.set("uMVMatrix", MV);
    shader.set("uNormalMatrix", glm::transpose(glm::inverse(MV)));

    // std::cout << (ProjMatrix * MV) << " proj * MV"
    //           << "\n \n";

    // std::cout << MV << " MV"
    //           << "\n \n";

    // std::cout << ProjMatrix << " proj"
    //           << "\n \n \n \n \n \n";
}

void set_material(p6::Shader& shader, const Material& material)
{
    shader.set("K_d", material.diffuse);
    shader.set("K_s", material.glossy);
    shader.set("shininess", material.shininess);
}

template<typename T> // works for point and directionnal light
void send_light(p6::Shader& shader, const std::vector<T>& list_light, const glm::mat4& view_mat, const bool& is_directionnal)
{
    lightDatas lights_datas = fill_light_data(list_light, view_mat);
    // std::cout << lights_datas.list_color.size << " // " << lights_datas.list_intensity.size << " // " << lights_datas.list_position_or_direction_view.size << "\n";
    if (is_directionnal)
    {
        send_light_dir_uniform(shader, lights_datas);
    }
    else
    {
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

    GLuint w_i  = glad_glGetUniformLocation(shader_id, "w_i");
    GLuint L_i     = glad_glGetUniformLocation(shader_id, "L_i");
    GLuint intensity = glad_glGetUniformLocation(shader_id, "intensity");

    // actually set them
    glUniform3fv(w_i, nb_light, (const GLfloat*)light_datas.position_or_direction_view.data());
    glUniform3fv(L_i, nb_light, (const GLfloat*)light_datas.list_color.data());
    glUniform1fv(intensity, nb_light, (const GLfloat*)light_datas.list_intensity.data());
    shader.set("nb_light", nb_light);
}

void send_light_dir_uniform(p6::Shader& shader, const lightDatas& light_datas)
{
    GLint  shader_id = shader.id();
    size_t nb_light  = light_datas.list_color.size();

    GLuint direction_i          = glad_glGetUniformLocation(shader_id, "direction_i");
    GLuint L_i_direction              = glad_glGetUniformLocation(shader_id, "L_i_direction");
    GLuint intensity_direction = glad_glGetUniformLocation(shader_id, "intensity_direction");

    // actually set them
    glUniform3fv(direction_i, nb_light, (const GLfloat*)light_datas.position_or_direction_view.data());
    glUniform3fv(L_i_direction, nb_light, (const GLfloat*)light_datas.list_color.data());
    glUniform1fv(intensity_direction, nb_light, (const GLfloat*)light_datas.list_intensity.data());
    shader.set("nb_light_directionnal", nb_light);
}

glm::vec4 get_position_or_direction_homogene(PointLight light)
{
    return glm::vec4(light.position,1.);
}

glm::vec4 get_position_or_direction_homogene(DirectionalLight light)
{
    return glm::vec4(light.direction,0.);
}

template<typename T>
lightDatas fill_light_data(const std::vector<T>& list_light, const glm::mat4& view_mat)
{
    lightDatas lights_datas;
    for (auto const& light : list_light)
    {
        glm::vec4 pos_or_dir_H = get_position_or_direction_homogene(light);
        glm::vec3 pos_or_dir_view = transform_in_view_space(glm::vec3(pos_or_dir_H), view_mat, pos_or_dir_H.w); // in view space

        lights_datas.position_or_direction_view.push_back(pos_or_dir_view);
        lights_datas.list_color.push_back(light.color);
        lights_datas.list_intensity.push_back(light.intensity);
    }
    return lights_datas;
}

void set_blinn_phong(Material& material, const std::vector<PointLight>& list_light, const std::vector<DirectionalLight>& list_directionnal_light, const glm::mat4& MV, const glm::mat4& ProjMatrix)
{
    set_matrix(material.shader, MV, ProjMatrix);
    set_material(material.shader, material);
    set_lights(material.shader, list_light, list_directionnal_light, MV);
}