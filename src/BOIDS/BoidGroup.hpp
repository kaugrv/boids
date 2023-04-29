#pragma once

#include <vector>
#include "Boid.hpp"
#include "p6/p6.h"

glm::vec3 random_vec3(float min, float max) {
    return glm::vec3(p6::random::number(min, max), p6::random::number(min, max), p6::random::number(min, max));
}

static Boid generate_random_boid() {
    return Boid(random_vec3(-0.9f, 0.9f), p6::random::number(-5.f, 5.f), glm::normalize(random_vec3(-1.f, 1.f)));
}

struct BoidGroupParameters {
    float m_cohesion{1.};
    float m_separation{0.};
    float m_alignment{0.};
    float m_radius{20.};
    int   m_boid_nb{1};
    bool  m_display_visual_range{false};
};

class BoidGroup {
private:
    BoidGroupParameters m_behavior{};

public:
    std::vector<Boid> m_boids{};

    BoidGroup() = default;
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
            const float dist = glm::distance(boid.m_position, other_boid.m_position);
            if (dist <= m_behavior.m_radius && &other_boid != &boid)
                neighbours_boids.push_back(other_boid);
        }
        return neighbours_boids;
    }

    // Cohesion, Seperation & Alignment return the average direction to all the neighbours
    glm::vec3 cohesion(const Boid& boid) 
    {
        if (get_neighbours(boid).empty())
            return glm::vec3(0);

        auto         cohesion_vector = glm::vec3(0);
        unsigned int neighbour_count = 0;

        for (auto& neighbour : get_neighbours(boid))
        {
            glm::vec3 difference = neighbour.m_position - boid.m_position;
            neighbour_count++;
            cohesion_vector += difference;
        }

        return glm::normalize(cohesion_vector / static_cast<float>(neighbour_count)) * m_behavior.m_cohesion;
    }

    glm::vec3 separation(const Boid& boid) 
    {
        if (get_neighbours(boid).empty())
            return glm::vec3(0);

        auto         separation_vector = glm::vec3(0);
        unsigned int neighbour_count   = 0;

        for (auto& neighbour : get_neighbours(boid))
        {
            float     dist = glm::distance(boid.m_position, neighbour.m_position);
            glm::vec3 difference{0};
            if (dist != 0)
                difference = -(neighbour.m_position - boid.m_position) / (dist * dist);
            neighbour_count++;
            separation_vector += difference;
        }

        return glm::normalize(separation_vector / static_cast<float>(neighbour_count)) * m_behavior.m_separation;
    }

    glm::vec3 alignment(const Boid& boid) 
    {
        if (get_neighbours(boid).empty())
            return glm::vec3(0);

        auto         alignment_vector = glm::vec3(0);
        unsigned int neighbour_count  = 0;

        for (auto& neighbour : get_neighbours(boid))
        {
            neighbour_count++;
            alignment_vector += neighbour.m_velocity;
        }

        return glm::normalize(alignment_vector / static_cast<float>(neighbour_count)) * m_behavior.m_alignment;
    }

    void avoid_obstacle(Boid& boid, const Obstacle& obstacle, float delta_time)
    {
        if (obstacle.is_bounded())
        {
            const float dist = obstacle.get_distance(boid.m_position);
            if (std::fabs(dist) >= 3.)
            {
                return;
            }

            const glm::vec3 normal = obstacle.get_normal(boid.m_position);

            boid.m_velocity += 200*delta_time * normal / dist;

            boid.m_velocity = glm::normalize(boid.m_velocity) * boid.m_speed;
        }
        else
        {
            const float abs_dist = std::fabs(obstacle.get_distance(boid.m_position));
            if (abs_dist >= 3.)
            {
                return;
            }

            const glm::vec3 normal = obstacle.get_normal( boid.m_position);

            boid.m_velocity += delta_time * normal / abs_dist;

            boid.m_velocity = glm::normalize(boid.m_velocity) *  boid.m_speed;
        }
    }

    void update_behavior(BoidGroupParameters gui)
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

    void update_all_boids(const float& delta_time, const std::vector<std::unique_ptr<Obstacle>>& obstacles)
    {
        update_boid_number();

        for (auto& boid : m_boids)
        {
            boid.set_direction(cohesion(boid) + separation(boid) + alignment(boid) + boid.m_direction);
            std::cout << cohesion(boid).x << cohesion(boid).y  << cohesion(boid).z << std::endl;
            

            // Check collisions with all obstacles of the scene (including bounds)
            for (auto const& obstacle : obstacles)
            {
                avoid_obstacle(boid, *obstacle, delta_time);
            }

            boid.update_position(delta_time);
        }
    }

    void reach_target(const float& follow_factor, const glm::vec3& target_position)
    {
        for (auto& boid : m_boids)
        {
            glm::vec3 dir = glm::normalize(target_position - boid.m_position);

            boid.set_direction(boid.m_position * (1 - follow_factor) + dir * follow_factor);
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