
#include <vector>
#include "3D_RENDER/light.hpp"
#include "Mesh.hpp"
#include "glm/gtx/transform.hpp"
#include "track_ball_camera.hpp"
#include "free_camera.hpp"
#include "Input_Movement.hpp"
#include "Material.hpp"
#include <vector> 
#include "light.hpp"
#include "shader_program.hpp"

struct Object3D {
    Mesh m_mesh;
    Material* m_material;

    glm::vec3 m_position;
    glm::vec3 m_rotation;
    glm::vec3 m_scale{1.};

    glm::mat4 getModelMatrix() {
        glm::mat4 ModelMatrix(1.);
        ModelMatrix = glm::translate(ModelMatrix, m_position);
        ModelMatrix = glm::rotate(ModelMatrix, glm::radians(m_rotation.x), glm::vec3(1.,0.,0.));
        ModelMatrix = glm::rotate(ModelMatrix, glm::radians(m_rotation.y), glm::vec3(0.,1.,0.));
        ModelMatrix = glm::rotate(ModelMatrix, glm::radians(m_rotation.z), glm::vec3(0.,0.,1.));
        ModelMatrix = glm::scale(ModelMatrix, m_scale);

        return ModelMatrix;
        
    }
};

struct ObjectsInScene {
    std::vector<Object3D> m_boids{};
    // Object3D m_surveyor{};
    // std::vector<Object3D> m_obstacles{};


};

struct Scene3D {

    // Cameras
    TrackballCamera m_trackBallCamera;
    FreeCamera      m_freeCam;  
    bool freecam_is_used = false;

    // Lights
    std::vector<PointLight>       m_list_point_light;
    std::vector<DirectionalLight> m_list_dir_light;

    // Objects in the scene
    ObjectsInScene m_objects_in_scene;



    // Constructor
    Scene3D() : m_trackBallCamera(-5, 0, 0), m_freeCam() {};

    void update_cameras(const Mouse& mouse, const MovementInput&keyboard, float delta_time) {
        m_trackBallCamera.updateTrackBallCamera(mouse.delta, mouse.is_left_button_pressed, mouse.is_right_button_pressed);
        m_freeCam.updateFreeCamera(delta_time, mouse, keyboard);
    }

    glm::mat4 getViewMatrix() {
        return freecam_is_used ? m_freeCam.getViewMatrix() : m_trackBallCamera.getViewMatrix() ;
    }

    glm::mat4 getProjMatrix(const p6::Context& ctx) {
        return freecam_is_used ? m_freeCam.getProjMatrix(ctx) : m_trackBallCamera.getProjMatrix(ctx) ;

    }

    void add_point_light(const PointLight& point_light) {
        m_list_point_light.push_back(point_light);
    }

    void add_dir_light(const DirectionalLight& dir_light) {
        m_list_dir_light.push_back(dir_light);
    }
    
    void add_boid(const Object3D& object) {
        m_objects_in_scene.m_boids.push_back(object);
    }


    void drawScene(const p6::Context& ctx) {

        for (auto& boid : m_objects_in_scene.m_boids) {
            
            boid.m_material->shader.use();
            glm::mat4 MVMatrix = getViewMatrix() * boid.getModelMatrix();
            set_blinn_phong(*boid.m_material, m_list_point_light, m_list_dir_light, MVMatrix, getProjMatrix(ctx));
            drawMesh(boid.m_mesh);
        }
    }
};

