// #include <winuser.h>
// BOIDS INCLUDE
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
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/scalar_constants.hpp"
#include "glm/fwd.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "3D_RENDER/Input_Movement.hpp"
#include "3D_RENDER/Material.hpp"
#include "3D_RENDER/Mesh.hpp"
#include "3D_RENDER/Texture.hpp"
#include "3D_RENDER/free_camera.hpp"
#include "3D_RENDER/random_sphere.hpp"
#include "3D_RENDER/shader_program.hpp"
#include "3D_RENDER/track_ball_camera.hpp"
#include "3D_RENDER/renderer.hpp"

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
    auto ctx = p6::Context{{.title = "Boids"}};

    // MainScene.add_obstacle(new Box(box));

    // Surveyor me;
    // float    follow_mouse_factor = 0.;

    // TO DO : MovementInput has keyboard and mouse
    MovementInput keyboard = MovementInput{};
    Mouse mouse = Mouse{};

    // SCENE
    Scene3D MainScene;

    // Create lights
    DirectionalLight dir_light{.direction = glm::vec3(0., -0.5, 0.), .color = glm::vec3(0.2, 0.58, 0.6), .intensity = 1.};
    PointLight point_light{.position = glm::vec3{0.2}, .color = glm::vec3(0.1, 0.7, 0.9), .intensity = 3.};

    // Push them into the scene
    MainScene.add_dir_light(dir_light);
    MainScene.add_point_light(point_light);

    // Texture
    p6::Image moon_image = p6::load_image("../assets/models/MoonMap.jpg");

    // GL options
    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_BACK);

    // Boid Object
    glimac::Cone cone(0.5, 0.3, 16, 32);
    Mesh mesh2(cone);
    Material material{glm::vec3(0.2, 1., 0.2), glm::vec3(0.5), glm::vec3(0.5), 2.};
    Object3D MYOBJECT {.m_mesh = mesh2, .m_material = &material};
    
    // Create group
    auto GUI = BoidGroupParameters{};
    BoidGroup group_of_boids(10);
    MainScene.m_objects_in_scene.m_group_of_boids = group_of_boids;

    // Bounding Box Object
    glimac::Sphere sphr(5., 16, 32);
    Mesh mesh(sphr);
    Material materialSphere{glm::vec3(0.1, 0.1, 0.), glm::vec3(0.5), glm::vec3(0.5), 2.};
    Object3D BOUND {.m_mesh = mesh, .m_material = &materialSphere};
    Sphere                 bounds{glm::vec3(0.), (5.), true};
    MainScene.add_obstacle(new Sphere(bounds));

    // Loop
    ctx.update = [&]() {
        
        ctx.background(p6::Color{0.f, 0.f, 0.5f});
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Input update
        keyboard.update_pressed_values(ctx);
        mouse.update_mouse(ctx);
        
        // Camera Update
        MainScene.update_cameras(mouse, keyboard, ctx.delta_time());

        // TO DO : GUI Window in Function
        //ImGui::ShowDemoWindow(); // Show the official ImGui demo window
        ImGui::Begin("Parameters");
        ImGui::SliderInt("Number of boids", &GUI.m_boid_nb, 0, 200);
        ImGui::SliderFloat("Cohesion", &GUI.m_cohesion, 0.f, 1.f);
        ImGui::SliderFloat("Separation", &GUI.m_separation, 0.f, 1.f);
        ImGui::SliderFloat("Alignment", &GUI.m_alignment, 0.f, 1.f);
        ImGui::SliderFloat("Visual range", &GUI.m_radius, 0.f, 0.5f);
        ImGui::Checkbox("Display visual range", &GUI.m_display_visual_range);
        // ImGui::SliderFloat("Mouse follow factor", &follow_mouse_factor, 0.f, 1.f);
        ImGui::Checkbox("Use Free Camera", &MainScene.freecam_is_used);
        ImGui::End();

        MainScene.m_objects_in_scene.m_group_of_boids.update_behavior(GUI); // Retrieve GUI slider and button changes
        MainScene.m_objects_in_scene.m_group_of_boids.update_all_boids(ctx.delta_time(), *MainScene.get_obstacles()); // Update all boids of the group
        MainScene.drawScene(ctx, MYOBJECT);
    };

    ctx.maximize_window();
    ctx.start();

};