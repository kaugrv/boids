#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <utility>
#include <vector>
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/scalar_constants.hpp"
#include "glm/fwd.hpp"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "Input_Movement.hpp"
#include "glm/gtx/transform.hpp"

class TrackballCamera {
private:
    // polar coordinates
    float m_fDistance;
    float m_fAngleX;
    float m_fAngleY;

public:
    TrackballCamera() = default;
    TrackballCamera(const float& dist, const float& angle_x, const float& angle_y)
        : m_fDistance(dist), m_fAngleX(angle_x), m_fAngleY(angle_y){};
    ~TrackballCamera() = default;

    void moveFront(const float& delta)
    {
        m_fDistance += delta;
    }

    void rotateLeft(const float& degree)
    {
        m_fAngleY += degree;
    }

    void rotateUp(const float& degree)
    {
        m_fAngleX += degree;
    }

    glm::mat4 getViewMatrix() const
    {
        glm::mat4 mv(1);
        mv = glm::translate(mv, glm::vec3(0., 0., m_fDistance));
        mv = glm::rotate(mv, m_fAngleY, glm::vec3(0., 1., 0.));
        mv = glm::rotate(mv, m_fAngleX, glm::vec3(1., 0., 0.));
        return mv;
    }
    void updateTrackBallCamera(float front_speed, glm::vec2 mouse_delta, float delta_time)
    {
        rotateLeft(mouse_delta.x * delta_time / 100);
        rotateUp(mouse_delta.y / delta_time / 100);
        moveFront(front_speed * delta_time);
    }

    friend std::ostream& operator<<(std::ostream& os, const TrackballCamera& cam);
};

// inline std::ostream& operator<<(std::ostream& os, const TrackballCamera& cam)
// {
//     os << cam.m_fDistance << '/' << cam.m_fAngleX << '/' << cam.m_fAngleY;
//     return os;
// }