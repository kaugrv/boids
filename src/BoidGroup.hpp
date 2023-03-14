#pragma once

#include <cstdlib>
#include <vector>
#include "glm/ext/quaternion_geometric.hpp"
#include "p6/p6.h"
#define DOCTEST_CONFIG_IMPLEMENT
#include <imgui.h>
#include "Boid.hpp"
#include "doctest/doctest.h"

struct BoidGroupBehavior {
    float m_cohesion;
    float m_separation;
    float m_alignment;
    float m_radius;
};

class BoidGroup {
private:
    std::vector<Boid> m_boids;
    BoidGroupBehavior m_behavior;

public:
    BoidGroup(const Boid& base_boid, const unsigned int& boid_number)
        : m_behavior{.m_cohesion = 0.5, .m_separation = 0.5, .m_alignment = 0.5, .m_radius = 0.5}
    {
        for (unsigned int i = 0; i < boid_number; i++)
        {
            m_boids.push_back(base_boid);
        }
    }

    BoidGroup(const unsigned int& boid_number)
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
        std::cout << neighbours_boids.size() << std::endl;
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

    void update_all_boids(const float& delta_time)
    {
        for (auto& boid : m_boids)
        {
            // separation
            // alignment

            // cohesion
            boid.set_direction(cohesion(boid) + boid.direction() + alignment(boid) + separation(boid));
            boid.update_position(delta_time);
            // std::cout << boid.position().x << std::endl;
        }
    }

    void draw_boids(p6::Context& ctx)
    {
        for (auto& boid : m_boids)
            boid.draw(ctx);
    }

    void reach_target(const float& follow_factor, const glm::vec2& target_position, const float& delta_time)
    {
        for (auto& boid : m_boids)
        {
            glm::vec2 dir = glm::normalize(target_position - boid.position());

            boid.set_direction(boid.direction() * (1 - follow_factor) + dir * follow_factor);
            // boid.update_position(delta_time);
        }
    }
};