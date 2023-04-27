#pragma once

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
#include "Object3D.hpp"
#include "./BOIDS/BoidGroup.hpp"
#include "./BOIDS/Sdf.hpp"



struct ObjectsInScene {
    BoidGroup m_group_of_boids{};
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
    ObjectsInScene m_objects_in_scene{};

    std::vector<std::unique_ptr<Obstacle>> m_Obstacles{};


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
    
    void add_boid(const Boid& boid) {
        m_objects_in_scene.m_group_of_boids.add_boid(boid);
    }

    auto get_obstacles() const
    {
        return &m_Obstacles;
    }

    void add_obstacle(Obstacle* obstacle_ptr)
    {
        if (obstacle_ptr != nullptr)
            m_Obstacles.push_back(std::unique_ptr<Obstacle>(obstacle_ptr));
    }

    void remove_obstacle()
    {
        m_Obstacles.pop_back();
    }


    void drawScene(const p6::Context& ctx, Object3D& object, Object3D& bound_box) {

        for (auto&& boid : m_objects_in_scene.m_group_of_boids.m_boids) {
            object.m_material->shader.use();
            glm::mat4 MVMatrix = getViewMatrix() * boid.getModelMatrix();
            set_blinn_phong(*object.m_material, m_list_point_light, m_list_dir_light, MVMatrix, getProjMatrix(ctx));
            drawMesh(object.m_mesh);
        }

        for (auto&& obstacle : m_Obstacles) {
            bound_box.m_material->shader.use();
            glm::mat4 MVMatrix = getViewMatrix() *glm::mat4(1.);
            set_blinn_phong(*bound_box.m_material, m_list_point_light, m_list_dir_light, MVMatrix, getProjMatrix(ctx));
            drawMesh(bound_box.m_mesh);
        }

    }
};

