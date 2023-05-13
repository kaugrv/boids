
#include <GLFW/glfw3.h>
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/vector_angle.hpp"
#include "glm/trigonometric.hpp"
#include "p6/p6.h"
#include "../Input/movement_input.hpp"

struct Surveyor {
    glm::vec3 m_position{0.};
    glm::vec3 m_rotation{0.};
    glm::vec3 m_scale{1.2};

    glm::mat4 getModelMatrix()
    {
        glm::mat4 ModelMatrix(1.0f);
        ModelMatrix = glm::translate(ModelMatrix, m_position);
        ModelMatrix = glm::rotate(ModelMatrix, m_rotation.x, glm::vec3(0.,1.,0.));
        ModelMatrix = glm::rotate(ModelMatrix, m_rotation.y, glm::vec3(1.,0.,0.));
        ModelMatrix = glm::rotate(ModelMatrix, m_rotation.z, glm::vec3(0.,0.,1.));
        ModelMatrix = glm::scale(ModelMatrix, m_scale);

        return ModelMatrix;
    }

    void update_position(float delta_time, Keyboard input) {
        if (m_position.x>=-0.9) {
            m_position.x -= input.right_pressed*delta_time;
        }
            m_rotation.x = m_position.x * glm::radians(90.);
            m_rotation.y = m_position.y * glm::radians(-75.);
            m_rotation.z = m_position.z * glm::radians(45.);

        if (m_position.x<=0.9) m_position.x += input.left_pressed*delta_time;
        if (m_position.y>=-0.9) m_position.y -= input.up_pressed*delta_time;
        if (m_position.y<=0.9) m_position.y += input.down_pressed*delta_time;
        if (m_position.z<=0.85) m_position.z += input.forward_pressed*delta_time;
        if (m_position.z>=-0.85) m_position.z -= input.backward_pressed*delta_time;
    }
};