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
#include "p6/p6.h"

// struture de la souris
struct Mouse {
    glm::dvec2 position{};
    glm::dvec2 delta{};
    double     scroll{};

    int is_left_button_pressed;
};

struct MovementInput {
    int forward_pressed;
    int backward_pressed;
    int left_pressed;
    int right_pressed;
    int up_pressed;
    int down_pressed;

    int forward_key  = GLFW_KEY_W;
    int backward_key = GLFW_KEY_S;
    int left_key     = GLFW_KEY_A;
    int right_key    = GLFW_KEY_D;
    int up_key       = GLFW_KEY_Q;
    int down_key     = GLFW_KEY_E;

    void update_pressed_values(const p6::Context& ctx)
    {
        forward_pressed  = ctx.key_is_pressed(forward_key);
        backward_pressed = ctx.key_is_pressed(backward_key);
        left_pressed     = ctx.key_is_pressed(left_key);
        right_pressed    = ctx.key_is_pressed(right_key);
        up_pressed       = ctx.key_is_pressed(up_key);
        down_pressed     = ctx.key_is_pressed(down_key);
    }
};
