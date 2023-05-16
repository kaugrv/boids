#include "free_camera.hpp"

void FreeCamera::updateFreeCamera(float delta_time, const MovementInput& input)
{
    // rotation
    if (input.m_mouse.is_left_button_pressed)
    {
        rotateLeft(-input.m_mouse.delta.x * delta_time * 500000.);
        rotateUp(input.m_mouse.delta.y * delta_time * 500000.);
    }

    // translation
    moveFront((input.m_keyboard.forward_pressed - input.m_keyboard.backward_pressed) * delta_time);
    moveLeft((input.m_keyboard.left_pressed - input.m_keyboard.right_pressed) * delta_time);
    moveUp((input.m_keyboard.up_pressed - input.m_keyboard.down_pressed) * delta_time);
    if (m_Position.x >= 0.9)
        m_Position.x = 0.89;
    if (m_Position.x <= -0.9)
        m_Position.x = -0.89;
    if (m_Position.y >= 0.9)
        m_Position.y = 0.89;
    if (m_Position.y <= -0.9)
        m_Position.y = -0.89;
    if (m_Position.z >= 0.9)
        m_Position.z = 0.89;
    if (m_Position.z <= -0.9)
        m_Position.z = -0.89;
}
