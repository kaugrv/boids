#include <cstdlib>
#include <vector>
#include "p6/p6.h"
#define DOCTEST_CONFIG_IMPLEMENT
#include <imgui.h>
#include "doctest/doctest.h"

class Boid {
private:
    glm::vec2 m_position;
    glm::vec2 m_velocity; // also direction
    float     m_speed;
    float     m_rotation;
    float     m_radius;

public:
    // constructors
    Boid() = default;
    explicit Boid(const glm::vec2& position, const float& speed, const float& rotation = 0., const float& radius = 0.01)
        : m_position(position), m_speed(speed), m_rotation(rotation), m_radius(radius), m_velocity(glm::vec2(cos(rotation) * speed, sin(rotation) * speed)){};

    // destructor
    ~Boid() = default;

    // getter
    glm::vec2 position()
    {
        return m_position;
    };

    float x()
    {
        return m_position.x;
    };
    float y()
    {
        return m_position.y;
    };

    float speed() const
    {
        return m_speed;
    };
    glm::vec2 direction()
    {
        return glm::normalize(m_velocity);
    }
    float rotation()
    {
        return m_rotation;
    };
    float radius()
    {
        return m_radius;
    };

    // setter
    void set_position(glm::vec2 position)
    {
        m_position = position;
    };
    void set_speed(glm::vec2 speed)
    {
        m_velocity = speed;
    };
    void set_rotation(float rotation)
    {
        m_rotation = rotation;
    };
    void set_radius(float radius)
    {
        m_radius = radius;
    };

    void bounce()
    {
        if (x() >= 1. || x() <= -1.)
        {
            m_velocity.x *= -1.;
            std::cout << "boing" << std::endl;
        }
        if (y() >= 1. || y() <= -1.)
        {
            m_velocity.y *= -1.;
            std::cout << "boing" << std::endl;
        }
    }

    void update_position(float delta_time)
    {
        bounce();
        m_position += delta_time * m_velocity;

        // std::cout << "Position : " << x() << " // " << y() << std::endl;

        // std::cout << "Speed : " << speed().x << " // " << speed().y << std::endl;

        // std::cout << delta_time << std::endl;
    }

    void draw(p6::Context& ctx)
    {
        ctx.square(
            p6::Center{
                x(), y()},
            p6::Radius{
                radius()}
        );
    }
};