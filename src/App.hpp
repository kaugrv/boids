#include "3D_RENDER/renderer.hpp"
#include "p6/p6.h"
#include "3D_RENDER/movement_input.hpp"

struct Application {
    p6::Context ctx = p6::Context{{.title = "Boids"}};
    MovementInput input = MovementInput{};
    Scene3D MainScene = Scene3D(ctx);

    Object3D m_car;
    Object3D m_box;

    BoidGroupParameters GUI = BoidGroupParameters{};
    PostProcessParameters post_processGUI = PostProcessParameters{};

    void initialize(){ 

        // Create lights
        DirectionalLight dir_light{.direction = glm::vec3(0., -0.5, 0.), .color = glm::vec3(0.2, 0.58, 0.6), .intensity = 1.};
        PointLight       point_light{.position = glm::vec3{0.}, .color = glm::vec3(0.1, 0.7, 0.9), .intensity = 0.1};
        
        // Push them into the scene
        MainScene.add_dir_light(dir_light);
        MainScene.add_point_light(point_light);

        // GL Options
        glEnable(GL_DEPTH_TEST);
        // glEnable(GL_CULL_FACE);
        //  glCullFace(GL_BACK);
        glDisable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Objects (Boids & Obstacle)
        std::vector<tinyobj::shape_t>    car_shapes;
        std::vector<tinyobj::material_t> car_materials;

        tinyobj::LoadObj(car_shapes, car_materials, "..\\assets\\models\\peugeot.obj");
        Mesh     car(car_shapes);
        auto car_mat_ptr = std::make_shared<Material>();
        car_mat_ptr.get()->parameters = {glm::vec3(0.2, 1., 0.2), glm::vec3(0.5), glm::vec3(0.5), 2., 1.};

        Object3D car_object {.m_mesh = car, .m_material = car_mat_ptr};


        BoidGroup group_of_boids(1);
        MainScene.m_objects_in_scene.m_group_of_boids = group_of_boids;

        // Bounding Box Object
        std::vector<tinyobj::shape_t>    box_shapes;
        std::vector<tinyobj::material_t> box_materials;

        tinyobj::LoadObj(box_shapes, box_materials, "..\\assets\\models\\cube.obj");
        Mesh     box_mesh(box_shapes);
        auto box_material_ptr = std::make_shared<Material>();
        box_material_ptr->parameters = {glm::vec3(0.2, 1., 0.2), glm::vec3(0.5), glm::vec3(0.5), 2., 0.5};

        // Object3D box;
        // box.m_mesh = box_mesh;
        // box.m_material = std::move(box_material_ptr);
        // Material box_material{glm::vec3(0.2, 1., 0.2), glm::vec3(0.5), glm::vec3(0.5), 2., 0.5};

        Object3D box{.m_mesh = box_mesh, .m_material = box_material_ptr};
        // m_box = box;

        Box bounds{glm::vec3(0.), glm::vec3(1.), true};
        MainScene.add_obstacle(new Box(bounds));

    };

    void update(){
        ctx.update = [&]() {
            ctx.background(p6::Color{0.f, 0.f, 0.5f});
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // Input update
            input.update_input(ctx);

            // Camera Update
            MainScene.update_cameras(input, ctx.delta_time());

            // TO DO : GUI Window in Function
            ImGui::Begin("Parameters");
            if(ImGui::CollapsingHeader("Boids Parameters"))
            {
                ImGui::SliderInt("Number of boids", &GUI.m_boid_nb, 0, 100);
                ImGui::SliderFloat("Cohesion", &GUI.m_cohesion, 0.f, 10.f);
                ImGui::SliderFloat("Separation", &GUI.m_separation, 0.f, 10.f);
                ImGui::SliderFloat("Alignment", &GUI.m_alignment, 0.f, 10.f);
                ImGui::SliderFloat("Visual range", &GUI.m_radius, 0.f, 0.5f);
                ImGui::Checkbox("Display visual range", &GUI.m_display_visual_range);
                // ImGui::SliderFloat("Mouse follow factor", &follow_mouse_factor, 0.f, 1.f);
            }

            ImGui::Checkbox("Use Free Camera", &MainScene.freecam_is_used);

            if(ImGui::CollapsingHeader("Post Processing")){
                if (ImGui::TreeNode("type of fog"))
                    {
                        std::vector<std::string> fogs_type_name{"squared exponential", "exponential","linear"};

                        static int selected = post_processGUI.m_fog_type;
                        for (int n = 0; n < 3; n++)
                        {
                            if (ImGui::Selectable(fogs_type_name[n].data(), selected == n))
                                selected = n;
                        }
                        post_processGUI.m_fog_type = selected;
                        ImGui::TreePop();
                    }
                ImGui::SliderFloat("fog density", &post_processGUI.m_fog_density, 0.f, 1.f);
                ImGui::SliderFloat("fog near plane", &post_processGUI.m_near_plane, 0.01f, 5.f);
                ImGui::SliderFloat("fog far plane", &post_processGUI.m_far_plane, 10.f, 300.f);
                ImGui::ColorPicker4("background color",&post_processGUI.m_background_color.r);
            }
            ImGui::End();

            MainScene.m_objects_in_scene.m_group_of_boids.update_behavior(GUI);                                                 // Retrieve GUI slider and button changes
            MainScene.m_post_process.update_from_GUI_parameters(post_processGUI);
            MainScene.m_objects_in_scene.m_group_of_boids.update_all_boids(ctx.delta_time(), *MainScene.get_obstacles()); // Update all boids of the group

            MainScene.drawFinaleScene(ctx, m_car, m_box);

        };
        ctx.maximize_window();
        ctx.start();
    };


};