// #include <winuser.h>
// BOIDS INCLUDE
// #include <gl/gl.h>
#include "BOIDS/Sdf.hpp"
#include "glimac/Sphere.hpp"
#include "glm/matrix.hpp"
#define DOCTEST_CONFIG_IMPLEMENT
#include <imgui.h>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <memory>
#include <vector>
#include "BOIDS/Boid.hpp"
#include "BOIDS/BoidGroup.hpp"
#include "doctest/doctest.h"
#include "p6/p6.h"

// 3D INCLUDE
//  #include <GL/glext.h>
#include <algorithm>
#include <array>
#include <cmath>
// #include <glimac/FilePath.hpp>
#include <utility>
// #include "glimac/Image.hpp"
#include "3D_RENDER/light.hpp"
#include "glimac/tiny_obj_loader.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/scalar_constants.hpp"
#include "glm/fwd.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "3D_RENDER/Material.hpp"
#include "3D_RENDER/Mesh.hpp"
#include "3D_RENDER/Texture.hpp"
#include "3D_RENDER/free_camera.hpp"
#include "3D_RENDER/movement_input.hpp"
#include "3D_RENDER/post_process.hpp"
#include "3D_RENDER/random_sphere.hpp"
#include "3D_RENDER/renderer.hpp"
#include "3D_RENDER/shader_program.hpp"
#include "3D_RENDER/track_ball_camera.hpp"

// TO DO : clean all includes

int main(int argc, char* argv[])
{
    { // Run the tests
        if (doctest::Context{}.run() != 0)
            return EXIT_FAILURE;
        // The CI does not have a GPU so it cannot run the rest of the code.
        const bool no_gpu_available = argc >= 2 && strcmp(argv[1], "-nogpu") == 0; // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
        if (no_gpu_available)
        {
            return EXIT_SUCCESS;
        }
    }

    // Actual app
    auto          ctx   = p6::Context{{.title = "Boids"}};
    MovementInput input = MovementInput{};

    // TO DO : add surveyor

    // SCENE
    // TO DO : main scene contains everything
    Scene3D MainScene(ctx);

    // Create lights
    DirectionalLight dir_light{.direction = glm::vec3(0., -0.5, 0.), .color = glm::vec3(0.2, 0.58, 0.6), .intensity = 1.};
    PointLight       point_light{.position = glm::vec3{0.}, .color = glm::vec3(0.1, 0.7, 0.9), .intensity = 0.1};

    // Push them into the scene
    MainScene.add_dir_light(dir_light);
    MainScene.add_point_light(point_light);

    // GL options
    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);
    //  glCullFace(GL_BACK);
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    std::vector<tinyobj::shape_t>    car_shapes;
    std::vector<tinyobj::material_t> car_materials;

    tinyobj::LoadObj(car_shapes, car_materials, "..\\assets\\models\\peugeot.obj");
    Mesh     car(car_shapes);
    Material car_material{glm::vec3(0.2, 1., 0.2), glm::vec3(0.5), glm::vec3(0.5), 2., 1.};
    // Material car_material(car_materials[0]);
    Object3D car_object{.m_mesh = car, .m_material = &car_material};

    // Create group
    auto      GUI = BoidGroupParameters{};
    BoidGroup group_of_boids(1);
    MainScene.m_objects_in_scene.m_group_of_boids = group_of_boids;

    // Bounding Box Object
    std::vector<tinyobj::shape_t>    box_shapes;
    std::vector<tinyobj::material_t> box_materials;

    tinyobj::LoadObj(box_shapes, box_materials, "..\\assets\\models\\cube.obj");
    Mesh     box_mesh(box_shapes);
    Material box_material{glm::vec3(0.2, 1., 0.2), glm::vec3(0.5), glm::vec3(0.5), 2., 0.5};

    Object3D box{.m_mesh = box_mesh, .m_material = &box_material};

    Box bounds{glm::vec3(0.), glm::vec3(1.), true};
    MainScene.add_obstacle(new Box(bounds));

    float d = 0.2;
    float s = 100;

    // Loop
    ctx.update = [&]() {
        ctx.background(p6::Color{0.f, 0.f, 0.5f});
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Input update
        input.update_input(ctx);

        // Camera Update
        MainScene.update_cameras(input, ctx.delta_time());

        // TO DO : GUI Window in Function
        ImGui::ShowDemoWindow(); // Show the official ImGui demo window
        ImGui::Begin("Parameters");
        ImGui::SliderInt("Number of boids", &GUI.m_boid_nb, 0, 100);
        ImGui::SliderFloat("Cohesion", &GUI.m_cohesion, 0.f, 10.f);
        ImGui::SliderFloat("Separation", &GUI.m_separation, 0.f, 10.f);
        ImGui::SliderFloat("Alignment", &GUI.m_alignment, 0.f, 10.f);
        ImGui::SliderFloat("Visual range", &GUI.m_radius, 0.f, 0.5f);

        ImGui::SliderFloat("Avoid distance", &d, 0.f, 1.f);
        ImGui::SliderFloat("Avoid strength", &s, -100.f, 100.f);

        ImGui::Checkbox("Display visual range", &GUI.m_display_visual_range);
        // ImGui::SliderFloat("Mouse follow factor", &follow_mouse_factor, 0.f, 1.f);
        ImGui::Checkbox("Use Free Camera", &MainScene.freecam_is_used);

        ImGui::End();



        MainScene.m_objects_in_scene.m_group_of_boids.update_behavior(GUI);                                                 // Retrieve GUI slider and button changes
        MainScene.m_objects_in_scene.m_group_of_boids.update_all_boids(ctx.delta_time(), *MainScene.get_obstacles(), d, s); // Update all boids of the group

        MainScene.drawFinaleScene(ctx, car_object, box);

    };

    ctx.maximize_window();
    ctx.start();
};