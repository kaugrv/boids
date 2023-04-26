#include "Mesh.hpp"
#include "track_ball_camera.hpp"
#include "free_camera.hpp"
#include "Input_Movement.hpp"

struct Scene3D {

    TrackballCamera m_trackBallCamera;
    FreeCamera      m_freeCam;  
    bool freecam_is_used = false;

    Scene3D() : m_trackBallCamera(-5, 0, 0), m_freeCam() {};

    void update_cameras(const Mouse& mouse, const MovementInput&keyboard, float delta_time) {
        m_trackBallCamera.updateTrackBallCamera(mouse.delta, mouse.is_left_button_pressed, mouse.is_right_button_pressed);
        m_freeCam.updateFreeCamera(delta_time, mouse, keyboard);
    }
};

// TODO : list de mesh
void draw3D(Mesh &mesh, glm::vec3 position, glm::vec3 rotation) {
    glBindVertexArray(mesh.get_vao()); // Bind Mesh
    glDrawArrays(GL_TRIANGLES, 0, mesh.get_vertex_count());
}