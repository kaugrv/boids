#pragma once

#define DOCTEST_CONFIG_IMPLEMENT
#include <imgui.h>
#include <cmath>
#include <cstdlib>
#include <vector>
#include "p6/p6.h"


float sd_box(glm::vec2 position, glm::vec2 box_position, glm::vec2 box_size)
{
    glm::vec2 d = abs(position - box_position) - box_size;
    float     a = glm::length(glm::vec2(fmax(d.x, 0.), fmax(d.y, 0.)));
    return a + fmin(fmax(d.x, d.y), 0.0);
}

float sd_circle(glm::vec2 position, glm::vec2 circle_position, float circle_radius) {
    return glm::length(position-circle_position) - circle_radius;
}


class Shape {
    protected:    
        glm::vec2 m_shape_position;
        bool m_bound;

    public :
        explicit Shape(glm::vec2 shape_position, bool bound = false): m_shape_position(shape_position), m_bound(bound){};
        virtual ~Shape() {};

        // Shape(const Shape& shape):m_shape_position(shape.m_shape_position),m_bound(shape.m_bound) {};
        Shape(const Shape& shape) = default;

        virtual float get_distance(const glm::vec2& m_position) const = 0;

        glm::vec2 get_normal(const glm::vec2& position) const  {
            const float     eps = 0.0001;
            const float     dx  = get_distance(position + glm::vec2(eps, 0)) - get_distance(position - glm::vec2(eps, 0));
            const float     dy  = get_distance(position + glm::vec2(0, eps)) - get_distance(position - glm::vec2(0, eps));
            const glm::vec2 grad{dx, dy};

            return glm::normalize(grad);
        }

        bool is_bounded() const {
            return m_bound;
        }

        virtual void draw(p6::Context& ctx) =0 ;


};

class Box: public Shape {
    private:
        glm::vec2 m_size;

    public:

        Box(const glm::vec2& box_pos, const glm::vec2& size, bool bound=false): Shape(box_pos, bound), m_size(size) {};
        Box(const Box& box): Shape(box), m_size(box.m_size){};

        ~Box() = default;

        float get_distance(const glm::vec2& position) const override {
            return sd_box(position, m_shape_position, m_size);
        }

        void draw(p6::Context& ctx) override {
            ctx.stroke_weight = 0.01;
            ctx.fill          = {0.f, 0.f, 0.f, 0.5f};
            ctx.rectangle(
                p6::Center{
                    m_shape_position},
                p6::Radii{
                    m_size}
            );
        }
};


class Circle: public Shape {
        private:
            float m_radius;
        
        public:

            Circle(const glm::vec2& circle_pos, const float& radius, bool bound=false): Shape(circle_pos, bound), m_radius(radius) {};
            Circle(const Circle& circle): Shape(circle), m_radius(circle.m_radius){};
            ~Circle() = default;



            float get_distance(const glm::vec2& position) const override {
                return sd_circle(position, m_shape_position, m_radius);
            }


            void draw(p6::Context& ctx) override {
                ctx.stroke_weight = 0.01;
                ctx.fill          = {0.f, 0.f, 0.f, 0.5f};
                ctx.circle(
                    p6::Center{
                        m_shape_position},
                    p6::Radius{
                        m_radius}
                );
            }

};