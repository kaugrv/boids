#pragma once

#include <cmath>
#include <iostream>
#include <utility>
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/scalar_constants.hpp"
#include "glm/fwd.hpp"
#define GLFW_INCLUDE_NONE
#include "Input_Movement.hpp"
#include "glm/gtx/transform.hpp"
#include "p6/p6.h"

class FreeCamera {
private:
    // polar coordinates
    glm::vec3 m_Position;
    float     m_fPhi;
    float     m_fTheta;

    glm::vec3 m_FrontVector;
    glm::vec3 m_LeftVector;
    glm::vec3 m_Upvector;

    void computeDirectionVectors();

public:
    FreeCamera()
    {
        m_Position = glm::vec3(0.);
        m_fPhi     = glm::pi<float>();
        m_fTheta   = 0.;

        computeDirectionVectors();
    }
    ~FreeCamera() = default;

    void moveLeft(const float& t)
    {
        m_Position += t * m_LeftVector;
    }

    void moveFront(const float& t)
    {
        m_Position += t * m_FrontVector;
    }

    void moveUp(const float& t)
    {
        m_Position += m_Upvector * t;
    }

    void rotateLeft(const float& degree)
    {
        float rad = glm::radians(degree);
        m_fTheta += rad;
        computeDirectionVectors();
    }

    void rotateFront(const float& degree)
    {
        float rad = glm::radians(degree);
        m_fPhi += rad;
        computeDirectionVectors();
    }

    glm::mat4 getViewMatrix() const
    {
        return glm::lookAt(m_Position, m_Position + m_FrontVector, m_Upvector);
    }

    void updateFreeCamera(const p6::Context& ctx, const MovementInput& keyboard);

    friend std::ostream& operator<<(std::ostream& os, const FreeCamera& cam);
};
