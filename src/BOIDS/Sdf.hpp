#pragma once

#include <cmath>
#include <vector>
#include "p6/p6.h"

// Thanks to Inigo Quilez https://iquilezles.org/articles/distfunctions/

float sd_box(const glm::vec3& position, const glm::vec3& box_position, const glm::vec3& box_size);
float sd_sphere(const glm::vec3& position, const glm::vec3& sphere_position, const float& sphere_radius);

class Obstacle {
protected:
    glm::vec3 m_obstacle_position{};
    bool      m_bound{false};

public:
    Obstacle() = default;
    explicit Obstacle(glm::vec3 obstacle_position, bool bound = false)
        : m_obstacle_position(obstacle_position), m_bound(bound){};
    Obstacle(const Obstacle& obstacle) = default;
    virtual ~Obstacle()                = default;

    virtual float get_distance(const glm::vec3& m_position) const = 0;

    glm::vec3 get_normal(const glm::vec3& position) const
    {
        const float     eps = 0.0001f;
        const float     dx  = get_distance(position + glm::vec3(eps, 0, 0)) - get_distance(position - glm::vec3(eps, 0, 0));
        const float     dy  = get_distance(position + glm::vec3(0, eps, 0)) - get_distance(position - glm::vec3(0, eps, 0));
        const float     dz  = get_distance(position + glm::vec3(0, 0, eps)) - get_distance(position - glm::vec3(0, 0, eps));
        const glm::vec3 grad{dx, dy, dz};

        return glm::normalize(grad);
    }

    bool is_bounded() const
    {
        return m_bound;
    }

};

class Box : public Obstacle {
private:
    glm::vec3 m_size{};

public:
    Box(const glm::vec3& box_pos, const glm::vec3& size, bool bound = false)
        : Obstacle(box_pos, bound), m_size(size){};
    Box(const Box& box) = default;

    ~Box() override = default;

    float get_distance(const glm::vec3& position) const override
    {
        return sd_box(position, m_obstacle_position, m_size);
    }

};

class Sphere : public Obstacle {
private:
    float m_radius{};

public:
    Sphere(const glm::vec3& sphere_pos, const float& radius, bool bound = false)
        : Obstacle(sphere_pos, bound), m_radius(radius){};
    Sphere(const Sphere& sphere) = default;
    ~Sphere() override           = default;

    float get_distance(const glm::vec3& position) const override
    {
        return sd_sphere(position, m_obstacle_position, m_radius);
    }

};