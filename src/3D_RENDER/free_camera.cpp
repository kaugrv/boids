#include "free_camera.hpp"

void FreeCamera::updateFreeCamera(const float& delta_time, const Mouse& mouse, const MovementInput keyboard)
{
    // rotation
    if (mouse.is_left_button_pressed == 1) {
        rotateLeft(-mouse.delta.y * delta_time * 20.);
        rotateFront(-mouse.delta.x * delta_time * 20.);
    }

    // translation
    moveFront((keyboard.forward_pressed - keyboard.backward_pressed) * delta_time * 3.);
    moveLeft((keyboard.left_pressed - keyboard.right_pressed) * delta_time * 3.);
    moveUp((keyboard.up_pressed - keyboard.down_pressed) * delta_time * 3.);
}

void FreeCamera::computeDirectionVectors()
{
    m_FrontVector = glm::vec3(cosf(m_fTheta) * sinf(m_fPhi),
                              sinf(m_fTheta),
                              cosf(m_fTheta) * cosf(m_fPhi));

    m_LeftVector = glm::vec3(sinf(m_fPhi + glm::pi<float>() / 2.),
                             0.,
                             cos(m_fPhi + glm::pi<float>() / 2.));

    m_Upvector = glm::cross(m_FrontVector, m_LeftVector);
}

inline std::ostream& operator<<(std::ostream& os, const FreeCamera& cam)
{
    os << cam.m_Position << "/ phi : " << cam.m_fPhi << "/ theta : " << cam.m_fTheta;
    return os;
}