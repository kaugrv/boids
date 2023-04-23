#include "free_camera.hpp"

void FreeCamera::updateFreeCamera(const p6::Context& ctx, const MovementInput& keyboard) // TODO : pass vec2 delta_mouse and delta_time instead of the ctx
{
    // rotation
    if (ctx.mouse_button_is_pressed(p6::Button::Left))
    {
        rotateLeft(-ctx.mouse_delta().y * ctx.delta_time() * 200.);
        rotateFront(-ctx.mouse_delta().x * ctx.delta_time() * 200.);
    }

    // translation
    moveFront((keyboard.forward_pressed - keyboard.backward_pressed) * ctx.delta_time() * 3.);
    moveLeft((keyboard.left_pressed - keyboard.right_pressed) * ctx.delta_time() * 3.);
    moveUp((keyboard.up_pressed - keyboard.down_pressed) * ctx.delta_time() * 30.);
}

void FreeCamera::computeDirectionVectors()
{
    m_FrontVector = glm::vec3(cosf(m_fTheta) * sinf(m_fPhi), sinf(m_fTheta), cosf(m_fTheta) * cosf(m_fPhi));

    m_LeftVector = glm::vec3(sinf(m_fPhi + glm::pi<float>() / 2.), 0., cos(m_fPhi + glm::pi<float>() / 2.));

    m_Upvector = glm::cross(m_FrontVector, m_LeftVector);
}

// inline std::ostream& operator<<(std::ostream& os, const FreeCamera& cam)
// {
//     os << cam.m_Position << "/ phi : " << cam.m_fPhi << "/ theta : " << cam.m_fTheta;
//     return os;
// }