#pragma once

#include <algorithm>
#include <iostream>
#include <array>
#include <cmath>
#include <utility>
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/scalar_constants.hpp"
#include "glm/fwd.hpp"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glimac/glm.hpp>
#include "glimac/Image.hpp"

struct Material {

    glm::vec3 diffuse;
    glm::vec3 reflexion;
    glm::vec3 glossy;
    float shininess;

    // int texture;

    friend std::ostream& operator<<(std::ostream& os, const Material& cam);


};


inline std::ostream& operator<<(std::ostream& os, const Material& mat)
{
    os << "diffuse : "<< mat.diffuse << std::endl;
    os << "reflexion : "<< mat.reflexion << std::endl;
    os << "glossy : "<< mat.glossy << std::endl;
    os << "shininess : "<< mat.shininess;

    return os;
}