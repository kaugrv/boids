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


#define PI 3.149592

class FreeCamera {

public:
  glm::vec3 m_Position;
  float m_fPhi;
  float m_fTheta;
  glm::vec3 m_FrontVector;
  glm::vec3 m_LeftVector;
  glm::vec3 m_UpVector;

  void computeDirectionVectors() {
    m_FrontVector =
        glm::vec3(cos(glm::radians(m_fTheta)) * sin(glm::radians(m_fPhi)),
                  sin(glm::radians(m_fTheta)),
                  cos(glm::radians(m_fTheta)) * cos(glm::radians(m_fPhi)));
    m_LeftVector = glm::vec3(sin(glm::radians(m_fPhi) + PI / 2.), 0.,
                             cos(glm::radians(m_fPhi) + PI / 2.));
    m_UpVector = glm::cross(m_FrontVector, m_LeftVector);
  }

  FreeCamera() : m_Position(glm::vec3(0.)), m_fPhi(PI), m_fTheta(0.) {
    computeDirectionVectors();
  };

  void moveFront(float t) { m_Position += t * m_FrontVector; }
  void moveLeft(float t) { m_Position += t * m_LeftVector; }
  void moveUp(float t) { m_Position += t* m_UpVector;}
  void rotateLeft(float degrees) { m_fPhi += glm::radians(degrees); }
  void rotateUp(float degrees) { m_fTheta += glm::radians(degrees); }

  glm::mat4 getViewMatrix() {
    glm::vec3 eye = m_Position;
    glm::vec3 point = m_Position + m_FrontVector;
    glm::vec3 up = m_UpVector;
    computeDirectionVectors();
    return glm::lookAt(eye, point, up);
  }

  void updateFreeCamera(float delta_time, const Mouse& mouse, const MovementInput& keyboard);
};