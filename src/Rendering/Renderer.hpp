#pragma once

#include <vector>
#include "./Boids/BoidGroup.hpp"
#include "./Camera/free_camera.hpp"
#include "./Camera/track_ball_camera.hpp"
#include "./Input/movement_input.hpp"
#include "Light.hpp"
#include "Material.hpp"
#include "Mesh.hpp"
#include "Object3D.hpp"
#include "PostProcessing/post_process.hpp"
#include "Sdf.hpp"
#include "Surveyor/Surveyor.hpp"
#include "glm/gtx/transform.hpp"
#include "shader_program.hpp"

struct ObjectsInScene {
    BoidGroup                              m_group_of_boids{};
    std::vector<std::unique_ptr<Obstacle>> m_obstacles{};
    Surveyor                               m_surveyor{};

    std::vector<Object3D> m_objects_3D{};
};

struct Scene3D {
    // Cameras
    TrackballCamera m_trackBallCamera;
    FreeCamera      m_freeCam;
    bool            freecam_is_used = false;

    // Lights
    std::vector<PointLight>       m_list_point_light;
    std::vector<DirectionalLight> m_list_dir_light;

    ObjectsInScene m_objects_in_scene{};

    PostProcess m_post_process;

    // Constructor
    Scene3D(p6::Context& ctx)
        : m_trackBallCamera(-5, 0, 0), m_freeCam(), m_post_process(ctx){};

    void update_cameras(const MovementInput& input, float delta_time)
    {
        freecam_is_used ? m_freeCam.updateFreeCamera(delta_time, input) : m_trackBallCamera.updateTrackBallCamera(input.m_mouse.delta, input.m_mouse.is_left_button_pressed, input.m_mouse.is_right_button_pressed);
    }

    glm::mat4 getViewMatrix()
    {
        return freecam_is_used ? m_freeCam.getViewMatrix() : m_trackBallCamera.getViewMatrix(m_objects_in_scene.m_surveyor.m_position);
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
        return &m_objects_in_scene.m_obstacles;
    }

    void add_obstacle(std::unique_ptr<Obstacle> obstacle_ptr)
    {
        if (obstacle_ptr != nullptr)
            m_objects_in_scene.m_obstacles.push_back(std::move(obstacle_ptr));
    }

    void remove_obstacle()
    {
        m_objects_in_scene.m_obstacles.pop_back();
    }

    void add_object_3D(Object3D& object)
    {
        m_objects_in_scene.m_objects_3D.push_back(object);
    }

    void drawScene(const p6::Context& ctx)
    {
        // Boids
        for (auto&& boid : m_objects_in_scene.m_group_of_boids.m_boids)
        {
            m_objects_in_scene.m_objects_3D[m_objects_in_scene.m_group_of_boids.m_behavior.m_LOD].drawObject(getViewMatrix(), boid.getModelMatrix(), getProjMatrix(ctx), m_list_point_light, m_list_dir_light);
        }

        // Obstacles
        for (int i = 0; i < m_objects_in_scene.m_obstacles.size() - 1; i++)
        {
            m_objects_in_scene.m_objects_3D[5].drawObject(getViewMatrix(), m_objects_in_scene.m_obstacles[1 + i]->getModelMatrix(), getProjMatrix(ctx), m_list_point_light, m_list_dir_light);
        }

        // Surveyor
        m_objects_in_scene.m_objects_3D[4].drawObject(getViewMatrix(), m_objects_in_scene.m_surveyor.getModelMatrix(), getProjMatrix(ctx), m_list_point_light, m_list_dir_light);

        // CUBE
        m_objects_in_scene.m_objects_3D[0].drawObject(getViewMatrix(), m_objects_in_scene.m_obstacles[0]->getModelMatrix(), getProjMatrix(ctx), m_list_point_light, m_list_dir_light);
    }

    void drawFinaleScene(const p6::Context& ctx)
    {
        // POST PROCESS TEST
        //     first I bind my frame buffer
        glBindFramebuffer(GL_FRAMEBUFFER, m_post_process.m_frame_buffer.m_frame_buffer_id);
        glClearColor(0.f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // we're not using the stencil buffer now
        glEnable(GL_DEPTH_TEST);

        drawScene(ctx);

        glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
        glDisable(GL_DEPTH_TEST);
        glClearColor(0.f, 1.0f, 1.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        m_post_process.m_shader.use();
        glBindVertexArray(m_post_process.m_quad.m_vao);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, m_post_process.m_frame_buffer.m_texture_object);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ctx.main_canvas_width(), ctx.main_canvas_height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        m_post_process.m_shader.set("screenTexture", 1);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_post_process.m_frame_buffer.m_depth_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, ctx.main_canvas_width(), ctx.main_canvas_height(), 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);

        m_post_process.m_shader.set("depthTexture", 0);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        glBindTexture(GL_TEXTURE_2D, 0);
    }
};
