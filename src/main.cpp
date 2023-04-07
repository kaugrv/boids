// #include <winuser.h>
#include "Sdf.hpp"
#define DOCTEST_CONFIG_IMPLEMENT
#include <imgui.h>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <vector>
#include "Boid.hpp"
#include "BoidGroup.hpp"
#include "Surveyor.hpp"
#include "doctest/doctest.h"
#include "p6/p6.h"

int main(int argc, char* argv[])
{
    { // Run the tests
        if (doctest::Context{}.run() != 0)
            return EXIT_FAILURE;
        // The CI does not have a GPU so it cannot run the rest of the code.
        const bool no_gpu_available = argc >= 2 && strcmp(argv[1], "-nogpu") == 0; // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
        if (no_gpu_available)
            return EXIT_SUCCESS;
    }

    // Actual app
    auto ctx = p6::Context{{.title = "Boids"}};
    auto GUI = BoidGroupBehavior{};

    // Create group
    BoidGroup group_of_boids(1);

    // Create scene
    Scene MainScene;

    Box                 bounds{glm::vec2(0.), glm::vec2(1.), true};
    Box                 box{glm::vec2(0.), glm::vec2(0.2, 0.1)};
    Circle              circle(glm::vec2(0.5, 0.5), 0.1);
    Circle              circle2(glm::vec2(-0.5, 0.5), 0.1);
    Circle              circle3(glm::vec2(-0.5, -0.5), 0.1);
    Circle              circle4(glm::vec2(0.5, -0.5), 0.1);
    EquilateralTriangle triangle(glm::vec2(0., 0.3), 0.2);

    MainScene.add_obstacle(new Box(bounds));
    MainScene.add_obstacle(new Box(box));
    MainScene.add_obstacle(new Circle(circle));
    MainScene.add_obstacle(new Circle(circle2));
    MainScene.add_obstacle(new Circle(circle3));
    MainScene.add_obstacle(new Circle(circle4));
    MainScene.add_obstacle(new EquilateralTriangle(triangle));

    // Mouse "boid"
    Surveyor  me;
    glm::vec2 mouse_position(0.);
    bool      is_following        = false;
    float     follow_mouse_factor = 0.;

    ctx.update = [&]() {
        ctx.background(p6::Color{1.f, 1.f, 1.f});
        mouse_position = ctx.mouse();

        // ImGui::ShowDemoWindow(); // Show the official ImGui demo window

        ImGui::Begin("Parameters");
        ImGui::SliderInt("Number of boids", &GUI.m_boid_nb, 0, 200);
        ImGui::SliderFloat("Cohesion", &GUI.m_cohesion, 0.f, 1.f);
        ImGui::SliderFloat("Separation", &GUI.m_separation, 0.f, 1.f);
        ImGui::SliderFloat("Alignment", &GUI.m_alignment, 0.f, 1.f);
        ImGui::SliderFloat("Visual range", &GUI.m_radius, 0.f, 0.5f);
        ImGui::Checkbox("Display visual range", &GUI.m_display_visual_range);
        ImGui::SliderFloat("Mouse follow factor", &follow_mouse_factor, 0.f, 1.f);
        ImGui::End();

        MainScene.draw(ctx);

        group_of_boids.update_behavior(GUI);                          // Retrieve GUI slider and button changes
        group_of_boids.update_all_boids(ctx.delta_time(), MainScene); // Update all boids of the group
        group_of_boids.draw_boids(ctx);

        if (ctx.mouse_button_is_pressed(p6::Button::Left))
            group_of_boids.reach_target(follow_mouse_factor, mouse_position);
        me.draw(ctx);
    };

    ctx.maximize_window();

    // Should be done last. It starts the infinite loop.
    ctx.start();
};