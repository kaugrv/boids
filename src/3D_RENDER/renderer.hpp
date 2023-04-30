#pragma once

#include <vector>
#include "./BOIDS/BoidGroup.hpp"
#include "./BOIDS/Sdf.hpp"
#include "3D_RENDER/light.hpp"
#include "3D_RENDER/post_process.hpp"
#include "Input_Movement.hpp"
#include "Material.hpp"
#include "Mesh.hpp"
#include "Object3D.hpp"
#include "free_camera.hpp"
#include "glm/gtx/transform.hpp"
#include "light.hpp"
#include "shader_program.hpp"
#include "track_ball_camera.hpp"

struct ObjectsInScene {
    BoidGroup m_group_of_boids{};
};

struct Scene3D {
    // Cameras
    TrackballCamera m_trackBallCamera;
    FreeCamera      m_freeCam;
    bool            freecam_is_used = false;

    // Lights
    std::vector<PointLight>       m_list_point_light;
    std::vector<DirectionalLight> m_list_dir_light;

    // Objects in the scene
    ObjectsInScene m_objects_in_scene{};

    std::vector<std::unique_ptr<Obstacle>> m_Obstacles{};

    PostProcess m_post_process;

    // Constructor
    Scene3D(p6::Context& ctx)
        : m_trackBallCamera(-5, 0, 0), m_freeCam(), m_post_process(ctx){};

    void update_cameras(const Mouse& mouse, const MovementInput& keyboard, float delta_time)
    {
        m_trackBallCamera.updateTrackBallCamera(mouse.delta, mouse.is_left_button_pressed, mouse.is_right_button_pressed);
        m_freeCam.updateFreeCamera(delta_time, mouse, keyboard);
    }

    glm::mat4 getViewMatrix()
    {
        return freecam_is_used ? m_freeCam.getViewMatrix() : m_trackBallCamera.getViewMatrix();
    }

    glm::mat4 getProjMatrix(const p6::Context& ctx)
    {
        return freecam_is_used ? m_freeCam.getProjMatrix(ctx) : m_trackBallCamera.getProjMatrix(ctx);
    }

    void add_point_light(const PointLight& point_light)
    {
        m_list_point_light.push_back(point_light);
    }

    void add_dir_light(const DirectionalLight& dir_light)
    {
        m_list_dir_light.push_back(dir_light);
    }

    void add_boid(const Boid& boid)
    {
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

    void drawScene(const p6::Context& ctx, Object3D& object)
    {
        for (auto&& boid : m_objects_in_scene.m_group_of_boids.m_boids)
        {
            object.m_material->shader.use();
            glm::mat4 MVMatrix = getViewMatrix() * boid.getModelMatrix();
            set_blinn_phong(*object.m_material, m_list_point_light, m_list_dir_light, MVMatrix, getProjMatrix(ctx));
            drawMesh(object.m_mesh);
        }
    }

    void drawFinaleScene(const p6::Context& ctx, Object3D& object)
    {
        // POST PROCESS TEST
        //     first I bind my frame buffer
        glBindFramebuffer(GL_FRAMEBUFFER, m_post_process.m_frame_buffer.m_frame_buffer_id);
        glClearColor(1.f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // we're not using the stencil buffer now
        glEnable(GL_DEPTH_TEST);

        drawScene(ctx, object);

        glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
        glDisable(GL_DEPTH_TEST);
        glClearColor(0.f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        m_post_process.m_shader.use();
        glBindVertexArray(m_post_process.m_quad.m_vao);
        glBindTexture(GL_TEXTURE_2D, m_post_process.m_frame_buffer.m_texture_object);
        m_post_process.m_shader.set("screenTexture", 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
};
