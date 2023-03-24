#pragma once

#define DOCTEST_CONFIG_IMPLEMENT
#include <imgui.h>
#include <cmath>
#include <cstdlib>
#include <vector>
#include "p6/p6.h"

float sdBox(glm::vec2 position, glm::vec2 box_position, glm::vec2 box_size)
{
    glm::vec2 d = abs(position - box_position) - box_size;
    float     a = glm::length(glm::vec2(fmax(d.x, 0.), fmax(d.y, 0.)));
    return a + fmin(fmax(d.x, d.y), 0.0);
}

class Box {
private:
    glm::vec2 m_box_position;
    glm::vec2 m_box_size;

public:
    Box(const glm::vec2& box_pos, const glm::vec2& size)
        : m_box_position(box_pos), m_box_size(size) {}

    float get_distance(const glm::vec2& position) const
    {
        return sdBox(position, m_box_position, m_box_size);
    }

    glm::vec2 get_normal(const glm::vec2& position) const
    {
        const float     eps = 0.0001;
        const float     dx  = get_distance(position + glm::vec2(eps, 0)) - get_distance(position - glm::vec2(eps, 0));
        const float     dy  = get_distance(position + glm::vec2(0, eps)) - get_distance(position - glm::vec2(0, eps));
        const glm::vec2 grad{dx, dy};

        return glm::normalize(grad);
    }

    void draw(p6::Context& ctx)
    {
        ctx.stroke_weight = 0.01;
        ctx.fill          = {0.f, 0.f, 0.f, 0.5f};
        ctx.rectangle(
            p6::Center{
                m_box_position},
            p6::Radii{
                m_box_size}
        );
    }
};

using BoundBox = Box;
