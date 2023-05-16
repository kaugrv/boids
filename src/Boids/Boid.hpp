#pragma once

#include "./Rendering/Object3D.hpp"
#include "./Rendering/Sdf.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include "glm/gtx/vector_angle.hpp"
#include "p6/p6.h"

struct Boid {
    glm::vec3 m_position{};
    glm::vec3 m_velocity{};
    glm::vec3 m_direction{};
    glm::vec3 m_rotation{0.};
    glm::vec3 m_scale{0.3};
    float     m_speed{};

    // Constructors
    Boid() = default;
    explicit Boid(const glm::vec3& position, const float& speed, const glm::vec3& direction = glm::vec3(0., -1., 0.)) // NOLINT
        : m_position(position), m_velocity(direction * speed), m_direction(direction), m_speed(speed){};

    // Destructor
    ~Boid() = default;

    // Setters
    void set_position(const glm::vec3& position) { m_position = position; }

    void set_velocity(const glm::vec3& velocity)
    {
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

    glm::mat4 getModelMatrix()
    {
        glm::mat4 ModelMatrix(1.0f);

        ModelMatrix = glm::translate(ModelMatrix, m_position);

        ModelMatrix = glm::rotate(ModelMatrix, static_cast<float>(m_position.x * glm::radians(90.)), glm::vec3(0., 1., 0.));
        ModelMatrix = glm::rotate(ModelMatrix, static_cast<float>(m_position.y * glm::radians(-75.)), glm::vec3(1., 0., 0.));

        ModelMatrix = glm::scale(ModelMatrix, m_scale);

        return ModelMatrix;
    }
};
