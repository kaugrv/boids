#include "free_camera.hpp"

void FreeCamera::updateFreeCamera(float delta_time, const Mouse& mouse, const MovementInput& keyboard) 
{
    // rotation
    if (mouse.is_left_button_pressed)
    {
        rotateLeft(mouse.delta.x * delta_time * 200.);
        rotateUp(mouse.delta.y * delta_time * 200.);
    }

    // translation
    moveFront((keyboard.forward_pressed - keyboard.backward_pressed) * delta_time * 3.);
    moveLeft((keyboard.left_pressed - keyboard.right_pressed) * delta_time * 3.);
    moveUp((keyboard.up_pressed - keyboard.down_pressed) * delta_time * 3.);
}
