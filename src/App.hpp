#include "3D_RENDER/renderer.hpp"
#include "p6/p6.h"
#include "3D_RENDER/movement_input.hpp"



void update_boid_GUI(BoidGroupParameters& boid_parameters){
    if(ImGui::CollapsingHeader("Boids Parameters"))
        {
            ImGui::SliderInt("Number of boids", &boid_parameters.m_boid_nb, 0, 100);
            ImGui::SliderFloat("Cohesion", &boid_parameters.m_cohesion, 0.f, 1.f);
            ImGui::SliderFloat("Separation", &boid_parameters.m_separation, 0.f, 1.f);
            ImGui::SliderFloat("Alignment", &boid_parameters.m_alignment, 0.f, 1.f);
            ImGui::SliderFloat("Visual range", &boid_parameters.m_radius, 0.f, 0.5f);
            ImGui::Checkbox("Display visual range", &boid_parameters.m_display_visual_range);
            // ImGui::SliderFloat("Mouse follow factor", &follow_mouse_factor, 0.f, 1.f);
        }
};


void update_fog_UI(FogParameters& fog_parameters){
    if (ImGui::TreeNode("type of fog"))
        {
            std::vector<std::string> fogs_type_name{"squared exponential", "exponential","linear"};

            static int selected = fog_parameters.m_fog_type;
            for (int n = 0; n < 3; n++)
            {
                if (ImGui::Selectable(fogs_type_name[n].data(), selected == n))
                    selected = n;
            }
            fog_parameters.m_fog_type = selected;
            ImGui::TreePop();
        }
        ImGui::SliderFloat("fog density", &fog_parameters.m_fog_density, 0.f, 1.f);
        ImGui::SliderFloat("fog near plane", &fog_parameters.m_near_plane, 0.01f, 5.f);
        ImGui::SliderFloat("fog far plane", &fog_parameters.m_far_plane, 10.f, 300.f);
        ImGui::ColorEdit3("background color",&fog_parameters.m_background_color.r);
}

void update_outline_UI(OutlineParameters& outline_parameters){
    ImGui::Checkbox("Outline",&outline_parameters.m_outline_is_activated);
    if(outline_parameters.m_outline_is_activated){
        ImGui::SliderFloat("threshold", &outline_parameters.m_threshold, 0.f, 1.f);
        ImGui::ColorEdit3("outline color", &outline_parameters.m_outline_color.x);
    }
}

void update_post_process_GUI(PostProcessParameters& post_process_parameters){
    if(ImGui::CollapsingHeader("Post Processing")){
        update_fog_UI(post_process_parameters.m_fog_param);
        update_outline_UI(post_process_parameters.m_outline_param);
    }
};

void update_GUI(BoidGroupParameters& boid_parameters,PostProcessParameters& post_process_parameters){
    update_boid_GUI(boid_parameters);
    update_post_process_GUI(post_process_parameters);
};


struct Application {
    p6::Context ctx = p6::Context{{.title = "Boids"}};
    MovementInput input = MovementInput{};
    Scene3D MainScene = Scene3D(ctx);

    Object3D m_car;
    Object3D m_box;

    BoidGroupParameters GUI = BoidGroupParameters{};
    PostProcessParameters post_processGUI = PostProcessParameters{};

    std::vector<std::shared_ptr<Material>> list_material_used;
    std::vector<std::shared_ptr<Mesh>> list_mesh_used;

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

        // Boids
        std::vector<tinyobj::shape_t>    car_shapes;
        std::vector<tinyobj::material_t> car_materials;
        tinyobj::LoadObj(car_shapes, car_materials, "..\\assets\\models\\peugeot.obj",  "..\\assets\\models\\peugeot.mtl");
        Mesh     car(car_shapes);
        auto car_mat_ptr = std::make_shared<Material>();
        car_mat_ptr.get()->parameters = {glm::vec3(0.2, 1., 0.2), glm::vec3(0.5), glm::vec3(0.5), 2., 1.};
        list_material_used.push_back(car_mat_ptr);
        list_mesh_used.push_back(std::make_shared<Mesh>(car));
        Object3D car_object {.m_mesh = list_mesh_used[0], .m_material = list_material_used[0]};
        m_car = car_object;

        BoidGroup group_of_boids(1);
        MainScene.m_objects_in_scene.m_group_of_boids = group_of_boids;

        // Bounding Box Object
        std::vector<tinyobj::shape_t>    box_shapes;
        std::vector<tinyobj::material_t> box_materials;
        tinyobj::LoadObj(box_shapes, box_materials, "..\\assets\\models\\cube.obj");
        Mesh     box_mesh(box_shapes);
        auto box_material_ptr = std::make_shared<Material>();
        box_material_ptr->parameters = {glm::vec3(0.2, 1., 0.2), glm::vec3(0.5), glm::vec3(0.5), 2., 0.5};
        list_material_used.push_back(box_material_ptr);
        list_mesh_used.push_back(std::make_shared<Mesh>(box_mesh));
        Object3D box{.m_mesh = list_mesh_used[1], .m_material = list_material_used[1]};
        m_box = box;

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
            ImGui::Checkbox("Use Free Camera", &MainScene.freecam_is_used);

            update_GUI(GUI,post_processGUI);
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


