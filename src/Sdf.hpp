#pragma once

#define DOCTEST_CONFIG_IMPLEMENT
#include <imgui.h>
#include <cmath>
#include <cstdlib>
#include <vector>
#include "p6/p6.h"

// Thanks to Inigo Quilez https://iquilezles.org/articles/distfunctions2d/

float sd_box(const glm::vec2& position, const glm::vec2& box_position, const glm::vec2& box_size)
{
    glm::vec2 d = abs(position - box_position) - box_size;
    float     a = glm::length(glm::vec2(fmax(d.x, 0.), fmax(d.y, 0.)));
    return a + fmin(fmax(d.x, d.y), 0.0);
}

float sd_circle(const glm::vec2& position, const glm::vec2& circle_position, const float& circle_radius)
{
    return glm::length(position - circle_position) - circle_radius;
}

float sd_eq_triangle(const glm::vec2& position, const glm::vec2& triangle_position, const float& triangle_size)
{
    const float k = sqrt(3.0);
    glm::vec2   p = position - triangle_position;
    p.x           = std::abs(p.x) - triangle_size;
    p.y           = p.y + triangle_size / k;
    if (p.x + k * p.y > 0.0)
    {
        p = glm::vec2(p.x - k * p.y, -k * p.x - p.y);
        p /= 2.0;
    }
    p.x -= std::clamp<double>(p.x, -2.0 * triangle_size, 0.0);
    return -glm::length(p) * glm::sign(p.y);
}

class Obstacle {
protected:
    glm::vec2 m_obstacle_position;
    bool      m_bound;

public:
    explicit Obstacle(glm::vec2 obstacle_position, bool bound = false)
        : m_obstacle_position(obstacle_position), m_bound(bound){};
    virtual ~Obstacle(){};

    // Obstacle(const Obstacle& obstacle):m_obstacle_position(obstacle.m_obstacle_position),m_bound(obstacle.m_bound) {};
    Obstacle(const Obstacle& obstacle) = default;

    virtual float get_distance(const glm::vec2& m_position) const = 0;

    glm::vec2 get_normal(const glm::vec2& position) const
    {
        const float     eps = 0.0001;
        const float     dx  = get_distance(position + glm::vec2(eps, 0)) - get_distance(position - glm::vec2(eps, 0));
        const float     dy  = get_distance(position + glm::vec2(0, eps)) - get_distance(position - glm::vec2(0, eps));
        const glm::vec2 grad{dx, dy};

        return glm::normalize(grad);
    }

    bool is_bounded() const
    {
        return m_bound;
    }

    virtual void draw(p6::Context& ctx) = 0;
};

class Box : public Obstacle {
private:
    glm::vec2 m_size;

public:
    Box(const glm::vec2& box_pos, const glm::vec2& size, bool bound = false)
        : Obstacle(box_pos, bound), m_size(size){};
    Box(const Box& box) = default;

    ~Box() = default;

    float get_distance(const glm::vec2& position) const override
    {
        return sd_box(position, m_obstacle_position, m_size);
    }

    void draw(p6::Context& ctx) override
    {
        ctx.stroke_weight = 0.01;
        ctx.fill          = {0.f, 0.f, 0.f, 0.5f};
        ctx.rectangle(
            p6::Center{
                m_obstacle_position},
            p6::Radii{
                m_size}
        );
    }
};

class Circle : public Obstacle {
private:
    float m_radius;

public:
    Circle(const glm::vec2& circle_pos, const float& radius, bool bound = false)
        : Obstacle(circle_pos, bound), m_radius(radius){};
    Circle(const Circle& circle) = default;
    ~Circle()                    = default;

    float get_distance(const glm::vec2& position) const override
    {
        return sd_circle(position, m_obstacle_position, m_radius);
    }

    void draw(p6::Context& ctx) override
    {
        ctx.stroke_weight = 0.01;
        ctx.fill          = {0.f, 0.f, 0.f, 0.5f};
        ctx.circle(
            p6::Center{
                m_obstacle_position},
            p6::Radius{
                m_radius}
        );
    }
};

class EquilateralTriangle : public Obstacle {
private:
    float m_size;

public:
    EquilateralTriangle(const glm::vec2& triangle_pos, const float& size, bool bound = false)
        : Obstacle(triangle_pos, bound), m_size(size){};
    EquilateralTriangle(const EquilateralTriangle& eq_triangle) = default;
    ~EquilateralTriangle()                                      = default;

    float get_distance(const glm::vec2& position) const override
    {
        return sd_eq_triangle(position, m_obstacle_position, m_size);
    }

    void draw(p6::Context& ctx) override
    {
        ctx.stroke_weight = 0.01;
        ctx.fill          = {0.f, 0.f, 0.f, 0.5f};
        ctx.equilateral_triangle(
            p6::Center{
                m_obstacle_position},
            p6::Radius{
                m_size},
            p6::Rotation{
                -30_degrees}
        );
    }
};