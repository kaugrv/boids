#pragma once

#include "Sdf.hpp"
#include "p6/p6.h"

struct Boid {
    glm::vec3 m_position{};
    glm::vec3 m_velocity{};
    glm::vec3 m_direction{};
    float     m_speed{};

    // Constructors
    Boid() = default;

    explicit Boid(const glm::vec3& position, const float& speed, const glm::vec3& direction = glm::vec3(0., -1., 0.)) // NOLINT
        : m_position(position), m_velocity(direction*speed), m_speed(speed), m_direction(direction){};

    // Destructor
    ~Boid() = default;

    // Setters
    void set_position(const glm::vec3& position) {m_position = position;}

    void set_velocity(const glm::vec3& velocity){
        m_velocity = velocity;
        m_speed    = glm::length(m_velocity);
    }

    void set_speed(const float& speed)
    {
        m_speed    = speed;
        m_velocity = glm::normalize(m_velocity) * speed;
    }

    void set_direction(const glm::vec3& direction)
    {
        m_direction = direction;
        set_velocity(glm::normalize(direction) * m_speed);
    }

    void update_position(float delta_time)
    {
        m_position += delta_time * m_velocity;
    }


};
