#pragma once

#define DOCTEST_CONFIG_IMPLEMENT
#include <imgui.h>
#include <cmath>
#include <cstdlib>
#include <vector>
#include "p6/p6.h"

float sdBox( glm::vec2 position, glm::vec2 box_position, glm::vec2 box_size)
{
    glm::vec2 d = abs(position-box_position)-box_size;
    float a =  glm::length(glm::vec2(fmax(d.x,0.),fmax(d.y,0.)));
    return a + fmin(fmax(d.x,d.y),0.0);
}