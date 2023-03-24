#pragma once

#include <imgui.h>
#include <cstdlib>
#include <vector>
#include "doctest/doctest.h"
#include "p6/p6.h"

class Surveyor {
private:
    glm::vec2 m_position;
    glm::vec2 m_angle;

public:
    Surveyor()  = default;
    ~Surveyor() = default;

    void draw(p6::Context& ctx)
    {
        m_angle = (ctx.mouse_delta() == glm::vec2(0.f) ? m_angle : ctx.mouse_delta());

        ctx.stroke_weight = 0.;
        ctx.fill          = {0.f, 0.f, 0.f, 1.f};
        ctx.equilateral_triangle(
            p6::Center{ctx.mouse()},
            p6::Radius{
                0.02f},
            p6::Angle{m_angle}
        );
    }
};
