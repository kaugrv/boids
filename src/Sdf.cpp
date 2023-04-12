#include "Sdf.hpp"

float sd_box(const glm::vec2& position, const glm::vec2& box_position, const glm::vec2& box_size)
{
    glm::vec2 d = abs(position - box_position) - box_size;
    float     a = glm::length(glm::vec2(fmax(d.x, 0.), fmax(d.y, 0.)));
    return a + static_cast<float>(fmin(fmax(d.x, d.y), 0.0));
}

float sd_circle(const glm::vec2& position, const glm::vec2& circle_position, const float& circle_radius)
{
    return glm::length(position - circle_position) - circle_radius;
}

float sd_eq_triangle(const glm::vec2& position, const glm::vec2& triangle_position, const float& triangle_size)
{
    const float k = sqrtf(3.0);
    glm::vec2   p = position - triangle_position;
    p.x           = std::abs(p.x) - triangle_size;
    p.y           = p.y + triangle_size / k;
    if (p.x + k * p.y > 0.0)
    {
        p = glm::vec2(p.x - k * p.y, -k * p.x - p.y);
        p /= 2.0;
    }
    p.x -= std::clamp<float>(p.x, -2.0f * triangle_size, 0.0);
    return -glm::length(p) * glm::sign(p.y);
}