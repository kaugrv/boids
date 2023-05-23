#define DOCTEST_CONFIG_IMPLEMENT
#include <imgui.h>
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>
#include "doctest/doctest.h"
#include "glimac/tiny_obj_loader.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/scalar_constants.hpp"
#include "glm/fwd.hpp"
#include "glm/matrix.hpp"
#include "p6/p6.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "Boids/Boid.hpp"
#include "Boids/BoidGroup.hpp"
#include "Camera/free_camera.hpp"
#include "Camera/track_ball_camera.hpp"
#include "Input/movement_input.hpp"
#include "PostProcessing/post_process.hpp"
#include "Rendering/Light.hpp"
#include "Rendering/Material.hpp"
#include "Rendering/Mesh.hpp"
#include "Rendering/Object3D.hpp"
#include "Rendering/Renderer.hpp"
#include "Rendering/Sdf.hpp"
#include "Rendering/shader_program.hpp"

void update_boid_GUI(BoidGroupParameters& boid_parameters)
{
    ImGui::SliderInt("Number of boids", &boid_parameters.m_boid_nb, 0, 200);
    ImGui::SliderInt("Level of details", &boid_parameters.m_LOD, 1, 3);
    ImGui::SliderFloat("Cohesion", &boid_parameters.m_cohesion, 0.f, 1.f);
    ImGui::SliderFloat("Separation", &boid_parameters.m_separation, 0.f, 1.f);
    ImGui::SliderFloat("Alignment", &boid_parameters.m_alignment, 0.f, 1.f);
    ImGui::SliderFloat("Visual range", &boid_parameters.m_radius, 0.f, 1.f);
};

void update_fog_UI(FogParameters& fog_parameters)
{
    if (ImGui::TreeNode("Type of fog"))
    {
        std::vector<std::string> fogs_type_name{"Squared exponential", "Exponential", "Linear"};

        static int selected = fog_parameters.m_fog_type;
        for (int n = 0; n < 3; n++)
        {
            if (ImGui::Selectable(fogs_type_name[n].data(), selected == n))
                selected = n;
        }
        fog_parameters.m_fog_type = selected;
        ImGui::TreePop();
    }
    ImGui::SliderFloat("Fog density", &fog_parameters.m_fog_density, 0.f, 1.f);
    ImGui::SliderFloat("Fog near plane", &fog_parameters.m_near_plane, 0.01f, 5.f);
    ImGui::SliderFloat("Fog far plane", &fog_parameters.m_far_plane, 10.f, 300.f);
    ImGui::ColorEdit3("Background color", &fog_parameters.m_background_color.r);
}

void update_outline_UI(OutlineParameters& outline_parameters)
{
    ImGui::Checkbox("Outline", &outline_parameters.m_outline_is_activated);
    if (outline_parameters.m_outline_is_activated)
    {
        ImGui::SliderFloat("Threshold", &outline_parameters.m_threshold, 0.f, 1.f);
        ImGui::ColorEdit3("Outline color", &outline_parameters.m_outline_color.x);
    }
}

void update_post_process_GUI(PostProcessParameters& post_process_parameters)
{
    if (ImGui::CollapsingHeader("Post Processing"))
    {
        ImGui::Checkbox("Active", &post_process_parameters.m_is_post_process_activated);

        if (post_process_parameters.m_is_post_process_activated)
        {
            ImGui::Spacing();
            update_fog_UI(post_process_parameters.m_fog_param);
            update_outline_UI(post_process_parameters.m_outline_param);
        }
    }
};

void update_GUI(BoidGroupParameters& boid_parameters, PostProcessParameters& post_process_parameters)
{
    update_boid_GUI(boid_parameters);
    update_post_process_GUI(post_process_parameters);
};

struct Application {
    p6::Context   ctx       = p6::Context{{.title = "Boids Runner"}};
    MovementInput input     = MovementInput{};
    Scene3D       MainScene = Scene3D(ctx);

    BoidGroupParameters   GUI             = BoidGroupParameters{};
    PostProcessParameters post_processGUI = PostProcessParameters{};

    std::vector<std::shared_ptr<p6::Shader>> list_shaders_used;
    std::vector<std::shared_ptr<p6::Image>>  list_images_used;
    // 0 = blank

    std::vector<std::shared_ptr<Mesh>> list_mesh_used;
    // 0 = Cube
    // 1 = Low Boid, 2 = Medium Boid, 3 = High Boid
    // 4 = Surveyor
    // Then = Obstacles

    void init_shaders()
    {
        std::shared_ptr<p6::Shader> default_shader = std::make_shared<p6::Shader>(p6::load_shader("../src/shaders/3D.vs.glsl", "../src/shaders/light.fs.glsl"));
        list_shaders_used.push_back(default_shader);
    }

    void init_textures()
    {
        list_images_used.push_back(std::make_shared<p6::Image>(p6::load_image("../assets/textures/blank.png"))); //0
        
        list_images_used.push_back(std::make_shared<p6::Image>(p6::load_image("../assets/textures/metal.png"))); //1
        list_images_used.push_back(std::make_shared<p6::Image>(p6::load_image("../assets/textures/test2.png"))); //2
        list_images_used.push_back(std::make_shared<p6::Image>(p6::load_image("../assets/textures/building.png"))); //3
    }

    void init_lights()
    {
        DirectionalLight dir_light{.direction = glm::vec3(0., 1., 0.), .color = glm::vec3(1., 1., 1.), .intensity = 0.5};
        DirectionalLight dir_light2{.direction = glm::vec3(-1., 1., 0.), .color = glm::vec3(1., 1., 1.), .intensity = 0.5};

        PointLight       point_light{.position = glm::vec3{0.8, 0., 0.}, .color = glm::vec3(1., 1., 1.), .intensity = 0.3};
        MainScene.add_dir_light(dir_light);
        MainScene.add_dir_light(dir_light2);
        MainScene.add_point_light(point_light);
    }

    void init_cube()
    {
        std::vector<tinyobj::shape_t>    box_shapes;
        std::vector<tinyobj::material_t> box_materials;
        tinyobj::LoadObj(box_shapes, box_materials, "../assets/models/cube.obj");
        Mesh box_mesh(box_shapes);

        list_mesh_used.push_back(std::make_shared<Mesh>(box_mesh));

        Material box_material(MaterialParameters{.diffuse = glm::vec3(0., 0., 1.), .reflexion = glm::vec3(0.5), .glossy = glm::vec3(0.5), .shininess = 2., .alpha = 0.5}, list_shaders_used[0], list_images_used[2]);

        Object3D box{.m_mesh = list_mesh_used[0], .m_material = box_material};
        MainScene.add_object_3D(box);

        Box bounds{glm::vec3(0.), glm::vec3(1.), true};
        MainScene.add_obstacle(std::make_unique<Box>(bounds));
    }

    void init_boids()
    {
        // Boids
        std::vector<tinyobj::shape_t>    car_shapes;
        std::vector<tinyobj::material_t> car_materials;

        Material car_material(MaterialParameters{.diffuse = glm::vec3(0., 0., 1.), .reflexion = glm::vec3(0.5), .glossy = glm::vec3(0.5), .shininess = 2., .alpha = 1.}, list_shaders_used[0], list_images_used[2]);

        // Low
        tinyobj::LoadObj(car_shapes, car_materials, "../assets/models/peugeot_low.obj");
        Mesh car_low(car_shapes);
        list_mesh_used.push_back(std::make_shared<Mesh>(car_low));
        Object3D car_object_low{.m_mesh = list_mesh_used[1], .m_material = car_material};
        MainScene.add_object_3D(car_object_low);

        // Mid
        tinyobj::LoadObj(car_shapes, car_materials, "../assets/models/peugeot_mid.obj");
        Mesh car_mid(car_shapes);
        list_mesh_used.push_back(std::make_shared<Mesh>(car_mid));
        Object3D car_object_mid{.m_mesh = list_mesh_used[2], .m_material = car_material};
        MainScene.add_object_3D(car_object_mid);

        // High
        tinyobj::LoadObj(car_shapes, car_materials, "../assets/models/peugeot_high.obj");
        Mesh car_high(car_shapes);
        list_mesh_used.push_back(std::make_shared<Mesh>(car_high));
        Object3D car_object_high{.m_mesh = list_mesh_used[3], .m_material = car_material};
        MainScene.add_object_3D(car_object_high);

        BoidGroup group_of_boids(1);
        MainScene.m_objects_in_scene.m_group_of_boids = group_of_boids;

    }

    void init_surveyor(){
        
        // Surveyor
        std::vector<tinyobj::shape_t>    surveyor_shapes;
        std::vector<tinyobj::material_t> surveyor_materials;

        tinyobj::LoadObj(surveyor_shapes, surveyor_materials, "../assets/models/2049.obj");
        Mesh surveyor_mesh(surveyor_shapes);
        list_mesh_used.push_back(std::make_shared<Mesh>(surveyor_mesh));

        Material surveyor_material(MaterialParameters{.diffuse = glm::vec3(0.5, 0., 0), .reflexion = glm::vec3(0.1), .glossy = glm::vec3(0.5), .shininess = 2., .alpha = 1.}, list_shaders_used[0], list_images_used[0]);

        Object3D surveyor_object{.m_mesh = list_mesh_used[4], .m_material = surveyor_material};
        MainScene.add_object_3D(surveyor_object);

        Surveyor surveyor;
        MainScene.m_objects_in_scene.m_surveyor = surveyor;
    }

    void init_obstacles()
    {
        // Obstacle (building)
        Material building_material(MaterialParameters{.diffuse = glm::vec3(0., 0., 1.), .reflexion = glm::vec3(0.5), .glossy = glm::vec3(0.5), .shininess = 0., .alpha = 1.}, list_shaders_used[0], list_images_used[0]);

        Object3D building{.m_mesh = list_mesh_used[0], .m_material = building_material};
        MainScene.add_object_3D(building);

        Box building_obstacle{glm::vec3(0., -0.5, 0.), glm::vec3(0.2, 0.5, 0.2), false};
        MainScene.add_obstacle(std::make_unique<Box>(building_obstacle));
    }

    void initialize()
    {
        // Load Shaders
        init_shaders();

        // Load Textures
        init_textures();

        // Create lights
        init_lights();

        // GL Options
        glDisable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glClear(GL_DEPTH_BUFFER_BIT);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        init_cube();
        init_boids();
        init_surveyor();
        init_obstacles();
    };

    void update()
    {
        ctx.update = [&]() {
            ctx.background(p6::Color{0.f, 0.f, 0.3f});
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // Input update
            input.update_input(ctx);

            // Camera Update
            MainScene.update_cameras(input, ctx.delta_time());

            // GUI
            ImGui::Begin("Parameters");
            ImGui::Checkbox("Use Free Camera", &MainScene.freecam_is_used);
            update_GUI(GUI, post_processGUI);
            ImGui::End();

            // Scene Update
            MainScene.m_objects_in_scene.m_group_of_boids.update_behavior(GUI); // Retrieve GUI slider and button changes
            MainScene.m_post_process.update_from_GUI_parameters(post_processGUI);
            MainScene.m_objects_in_scene.m_group_of_boids.update_all_boids(ctx.delta_time(), *MainScene.get_obstacles()); // Update all boids of the group
            MainScene.m_objects_in_scene.m_surveyor.update_position(ctx.delta_time(), input.m_keyboard, MainScene.freecam_is_used, MainScene.m_freeCam.get_position());

            MainScene.m_list_point_light[0].updatae_light_position(MainScene.m_objects_in_scene.m_surveyor.m_position);

            // Draw
            post_processGUI.m_is_post_process_activated ? MainScene.drawFinaleScene(ctx) : MainScene.drawScene(ctx);
        };
        ctx.maximize_window();
        ctx.start();
    };
};
