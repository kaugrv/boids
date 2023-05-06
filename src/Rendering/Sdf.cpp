#include "Sdf.hpp"

float sd_box(const glm::vec3& position, const glm::vec3& box_position, const glm::vec3& box_size)
{
    glm::vec3 d = abs(position - box_position) - box_size;
    float     a = glm::length(glm::vec3(fmax(d.x, 0.), fmax(d.y, 0.), fmax(d.z, 0.)));
    return a + static_cast<float>(fmin(fmax(d.x, fmax(d.y, d.z)), 0.0));
}

float sd_sphere(const glm::vec3& position, const glm::vec3& sphere_position, const float& sphere_radius)
{
    return glm::length(position - sphere_position) - sphere_radius;
}