#pragma once

#include <vector>
#include "Boid.hpp"
#include "Scene.hpp"
#include "p6/p6.h"

static Boid generate_random_boid()
{
    return Boid(glm::vec2(p6::random::number(-0.9f, 0.9f), p6::random::number(-0.9f, 0.9f)), p6::random::number(-0.5f, 0.5f), p6::random::number(0.f, 2.f * p6::PI), 0.02f);
}

struct BoidGroupBehavior {
    float m_cohesion{0.5};
    float m_separation{0.5};
    float m_alignment{0.5};
    float m_radius{0.25};
    int   m_boid_nb{20};
    bool  m_display_visual_range{false};
};

class BoidGroup {
private:
    std::vector<Boid> m_boids{};
    BoidGroupBehavior m_behavior{};

public:
    BoidGroup(const Boid& base_boid, const unsigned int& boid_number)
    {
        for (unsigned int i = 0; i < boid_number; i++)
        {
            add_boid(base_boid);
        }
    }

    explicit BoidGroup(const unsigned int& boid_number)
    {
        for (unsigned int i = 0; i < boid_number; i++)
        {
            m_boids.push_back(generate_random_boid());
        }
    }

    std::vector<Boid> get_neighbours(const Boid& boid)
    {
        std::vector<Boid> neighbours_boids{};

        for (auto& other_boid : m_boids)
        {
            const float dist = glm::distance(boid.position(), other_boid.position());
            if (dist <= m_behavior.m_radius && &other_boid != &boid)
                neighbours_boids.push_back(other_boid);
        }
        return neighbours_boids;
    }

    glm::vec2 cohesion(const Boid& boid) // return the average direction to all the neighbours
    {
        if (get_neighbours(boid).empty())
            return glm::vec2(0);

        auto         cohesion_vector = glm::vec2(0);
        unsigned int neighbour_count = 0;

        for (auto& neighbour : get_neighbours(boid))
        {
            glm::vec2 difference = neighbour.position() - boid.position();
            neighbour_count++;
            cohesion_vector += difference;
        }

        return glm::normalize(cohesion_vector / static_cast<float>(neighbour_count)) * m_behavior.m_cohesion;
    }

    glm::vec2 separation(const Boid& boid) // return the average direction to all the neighbours
    {
        if (get_neighbours(boid).empty())
            return glm::vec2(0);

        auto         separation_vector = glm::vec2(0);
        unsigned int neighbour_count   = 0;

        for (auto& neighbour : get_neighbours(boid))
        {
            float     dist = glm::distance(boid.position(), neighbour.position());
            glm::vec2 difference{0};
            if (dist != 0)
                difference = -(neighbour.position() - boid.position()) / (dist * dist);
            neighbour_count++;
            separation_vector += difference;
        }

        return glm::normalize(separation_vector / static_cast<float>(neighbour_count)) * m_behavior.m_separation;
    }

    glm::vec2 alignment(const Boid& boid) // return the average direction to all the neighbours
    {
        if (get_neighbours(boid).empty())
            return glm::vec2(0);

        auto         alignment_vector = glm::vec2(0);
        unsigned int neighbour_count  = 0;

        for (auto& neighbour : get_neighbours(boid))
        {
            neighbour_count++;
            alignment_vector += neighbour.velocity();
        }

        return glm::normalize(alignment_vector / static_cast<float>(neighbour_count)) * m_behavior.m_alignment;
    }

    void update_behavior(BoidGroupBehavior gui)
    {
        m_behavior = gui;
    }

    void update_boid_number()
    {
        while (m_boids.size() > static_cast<size_t>(m_behavior.m_boid_nb))
        {
            remove_boid();
        }
        while (m_boids.size() < static_cast<size_t>(m_behavior.m_boid_nb))
        {
            add_boid(generate_random_boid());
        }
    }

    void update_all_boids(const float& delta_time, const Scene& scene)
    {
        update_boid_number();

        for (auto& boid : m_boids)
        {
            boid.set_direction(cohesion(boid) + separation(boid) + alignment(boid) + boid.direction());

            // Check collisions with all obstacles of the scene (including bounds)
            for (auto const& obstacle : *scene.get_obstacles())
            {
                boid.avoid_obstacle(*obstacle, delta_time);
            }

            boid.update_position(delta_time);
        }
    }

    void draw_boids(p6::Context& ctx)
    {
        for (auto& boid : m_boids)
            boid.draw(ctx);
        draw_visual_range(ctx);
    }

    void draw_visual_range(p6::Context& ctx)
    {
        if (m_behavior.m_display_visual_range)
        {
            for (auto& boid : m_boids)
            {
                ctx.stroke_weight = 0.;
                ctx.fill          = {1.f, 1.f, 1.f, 0.1f};
                ctx.circle(
                    p6::Center{
                        boid.x(), boid.y()},
                    p6::Radius{
                        m_behavior.m_radius}
                );
            }
        }
    }

    void reach_target(const float& follow_factor, const glm::vec2& target_position)
    {
        for (auto& boid : m_boids)
        {
            glm::vec2 dir = glm::normalize(target_position - boid.position());

            boid.set_direction(boid.direction() * (1 - follow_factor) + dir * follow_factor);
        }
    }

    void add_boid(const Boid& boid)
    {
        m_boids.push_back(boid);
    }

    void remove_boid()
    {
        m_boids.pop_back();
    }
};