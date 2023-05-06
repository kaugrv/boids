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
#include "./Input/movement_input.hpp"
#include "glm/gtx/transform.hpp"
#include "p6/p6.h"

class TrackballCamera {
private:
    float m_fDistance;
    float m_fAngleX;
    float m_fAngleY;

    float m_fov  = glm::radians(70.f);
    float m_near = 0.1f;
    float m_far  = 100.f;

public:
    TrackballCamera() = default;
    TrackballCamera(const float& dist, const float& angle_x, const float& angle_y)
        : m_fDistance(dist), m_fAngleX(angle_x), m_fAngleY(angle_y){};
    ~TrackballCamera() = default;

    void moveFront(const float& delta) { m_fDistance += delta; }
    void rotateLeft(const float& degree) { m_fAngleY += degree; }
    void rotateUp(const float& degree) { m_fAngleX += degree; }

    glm::mat4 getViewMatrix() const
    {
        glm::mat4 V(1);
        V = glm::translate(V, glm::vec3(0., 0., m_fDistance));
        V = glm::rotate(V, m_fAngleY, glm::vec3(0., 1., 0.));
        V = glm::rotate(V, m_fAngleX, glm::vec3(1., 0., 0.));
        return V;
    }

    void updateTrackBallCamera(glm::vec2 mouse_delta, bool is_left_button_pressed, bool is_right_button_pressed)
    {
        if (is_left_button_pressed)
        {
            rotateLeft(mouse_delta.x);
            rotateUp(mouse_delta.y);
        }

        if (is_right_button_pressed)
        {
            moveFront(mouse_delta.y);
        }
    }

    glm::mat4 getProjMatrix(const p6::Context& ctx)
    {
        return glm::perspective<float>(m_fov, ctx.aspect_ratio(), m_near, m_far);
    }
};
