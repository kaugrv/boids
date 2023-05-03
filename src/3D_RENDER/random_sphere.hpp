#include <algorithm>
#include <array>
#include <cmath>
#include <utility>
#include <vector>
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/scalar_constants.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/random.hpp"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "glimac/Sphere.hpp"
#include "glm/gtx/transform.hpp"

std::vector<glm::vec3>
    generate_spherical_vector(const unsigned int& amount, const float& radius)
{
    std::vector<glm::vec3> list_of_axis;
    for (unsigned int i = 0; i < amount; i++)
    {
        list_of_axis.push_back(glm::sphericalRand(radius));
    }
    return list_of_axis;
}

glm::mat4 get_transformations(const float& rotation, const glm::mat4& mv_matrix, const glm::vec3& rotate_axis, const glm::vec3& start_pos)
{
    glm::mat4 mv2;
    mv2 = glm::rotate(mv_matrix, rotation, rotate_axis); // Translation * Rotation
    mv2 = glm::translate(mv2, start_pos);                // Translation * Rotation * Translation
    mv2 = glm::scale(mv2, glm::vec3(0.5, 0.5, 0.5));     // Translation * Rotation * Translation * Scale
    return mv2;
}