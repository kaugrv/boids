// // #include <GL/glext.h>
// #include <algorithm>
// #include <array>
// #include <cmath>
// #include <utility>
// #include <vector>
// #include "glimac/Image.hpp"
// #include "glm/ext/matrix_clip_space.hpp"
// #include "glm/ext/scalar_constants.hpp"
// #include "glm/fwd.hpp"
// #include "light.hpp"
// #define GLFW_INCLUDE_NONE
// #include <GLFW/glfw3.h>
// #include <glad/glad.h>
// #include <glimac/FilePath.hpp>
// #include <glimac/Program.hpp>
// #include <glimac/Sphere.hpp>
// #include <glimac/glm.hpp>
// #include "Input_Movement.hpp"
// #include "Material.hpp"
// #include "Mesh.hpp"
// #include "Texture.hpp"
// #include "free_camera.hpp"
// #include "random_sphere.hpp"
// #include "shader_program.hpp"
// #include "track_ball_camera.hpp"

// // no need
// int window_width  = 800;
// int window_height = 600;

// // already exists
// static Mouse mouse{};

// // usefull ?
// static MovementInput keyboard = MovementInput{
//     .forward_key  = GLFW_KEY_W,
//     .backward_key = GLFW_KEY_S,
//     .left_key     = GLFW_KEY_A,
//     .right_key    = GLFW_KEY_D,
//     .up_key       = GLFW_KEY_Q,
//     .down_key     = GLFW_KEY_E};

// // keep it
// TrackballCamera trackBallCamera = TrackballCamera(-5, 0, 0);
// FreeCamera      freeCam         = FreeCamera();

// void updateTrackBallCamera(TrackballCamera& cam, double delta = 0.);
// void debug_movement_input(MovementInput input);

// // void mouse_button_callback([[maybe_unused]] GLFWwindow* window, int button, int action, [[maybe_unused]] int mods)
// // {
// //     if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_REPEAT)
// //     {
// //         // std::cout << "clic" << std::endl;
// //         trackBallCamera.rotateLeft(mouse.delta.x);
// //         trackBallCamera.rotateUp(mouse.delta.y);

// //         // std::cout << trackBallCamera << std::endl
// //         // << std::endl;
// //     }
// // } // TO CHANGE => another way with p6

// // static void scroll_callback(GLFWwindow* /*window*/, [[maybe_unused]] double xoffset, [[maybe_unused]] double yoffset)
// // {
// //     mouse.scroll = yoffset;
// //     trackBallCamera.updateTrackBallCamera(yoffset, mouse);

// //     std::cout << mouse.scroll << std::endl;
// // } TO CHANGE => an other way with p6

// static float aspectRatio = 1.f * window_width / window_height;

// int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
// {
//     // setup the shaders
//     glimac::FilePath applicationPath(argv[0]);

//     EarthProgram      earthProgram(applicationPath);
//     MoonProgram       moonProgram(applicationPath);
//     BlinnPhongProgram blinnPhongProgram(applicationPath);

//     glimac::FilePath relative_path = applicationPath.dirPath() + ".." + ".." + "..";

//     glEnable(GL_DEPTH_TEST);

//     // MATRIX
//     glm::mat4 ProjMatrix;
//     glm::mat4 MVMatrix;
//     glm::mat4 NormalMatrix;

//     glm::mat4 MVPMatrix;

//     // calculate them
//     MVMatrix = glm::translate(glm::mat4(1), glm::vec3(0, 0, -5));

//     ProjMatrix = glm::perspective<float>(glm::radians(70.f), aspectRatio, 0.1f, 100.f);

//     MVPMatrix    = ProjMatrix * MVMatrix;
//     NormalMatrix = glm::transpose(glm::inverse(MVMatrix)); // transforms tha affect normals

//     ///// DATAS
//     glimac::Sphere sphr(1, 16, 32);
//     Mesh           mesh(sphr);

//     /// TEXTURES
//     Texture moon_texture(relative_path + "assets/texture/MoonMap.jpg");

//     Texture earth_texture(relative_path + "assets/texture/EarthMap.jpg");

//     Texture cloud_texture(relative_path + "assets/texture/CloudMap.jpg");

//     // add a light
//     // PointLight light{.position  = glm::vec3(3., 2., 0.),
//     //                  .color     = glm::vec3(1., 0., 0.),
//     //                  .intensity = 3.};

//     // PointLight light2{.position  = glm::vec3(0.),
//     //                   .color     = glm::vec3(0, 0, 1),
//     //                   .intensity = 3.};

//     DirectionalLight dir_light{.direction = glm::vec3(0., -0.5, 0.), .color = glm::vec3(0.2, 0.58, 0.6), .intensity = 1.};

//     /// push them into the list
//     std::vector<PointLight> list_light;
//     // list_light.push_back(light);
//     // list_light.push_back(light2);

//     std::vector<DirectionalLight> list_dir_light;
//     list_dir_light.push_back(dir_light);

//     // add a material for the moon
//     Material material{.diffuse = glm::vec3(0.2, 1., 0.2), .reflexion = glm::vec3(0.5), .glossy = glm::vec3(0.5), .shininess = 2.};

//     [[maybe_unused]] float delta_time = 0.;

//     auto list_axis = generate_spherical_vector(10, 0.5);
//     auto list_pos  = generate_spherical_vector(10, 2);

//     glm::dvec2 mousePosStart;
//     glm::dvec2 mousePosEnd;

//     glEnable(GL_CULL_FACE);
//     glCullFace(GL_BACK);

//     /* Loop until the user closes the window */
//     while (!glfwWindowShouldClose(window))
//     {
//         mouse.is_left_button_pressed = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
//         keyboard.update_pressed_values(window);

//         if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT))
//         {
//             trackBallCamera.updateTrackBallCamera(0.5, mouse);
//         }

//         debug_movement_input(keyboard);

//         glfwGetCursorPos(window, &mouse.position.x, &mouse.position.y);
//         mousePosStart = mouse.position;

//         float start_time = (float)glfwGetTime();

//         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//         glClearColor(1.f, 0.f, 1.f, 1.f);

//         // EARTH
//         earthProgram.m_Program.use();
//         glUniform1i(earthProgram.uEarthTexture, 0);
//         glUniform1i(earthProgram.uCloudTexture, 1);

//         freeCam.updateFreeCamera(delta_time, mouse, keyboard);
//         // MVMatrix = trackBallCamera.getViewMatrix();

//         MVMatrix     = freeCam.getViewMatrix();
//         MVPMatrix    = ProjMatrix * MVMatrix;
//         NormalMatrix = glm::transpose(glm::inverse(MVMatrix)); // transforms tha affect normals

//         glm::mat4 earthMVMatrix = glm::rotate(MVMatrix, start_time, glm::vec3(0, 1, 0));
//         glUniformMatrix4fv(earthProgram.uMVMatrix, 1, GL_FALSE, glm::value_ptr(earthMVMatrix));
//         glUniformMatrix4fv(earthProgram.uNormalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(earthMVMatrix))));
//         glUniformMatrix4fv(earthProgram.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(ProjMatrix * earthMVMatrix));

//         glBindVertexArray(mesh.get_vao());
//         // set the cloud :
//         glActiveTexture(GL_TEXTURE1);
//         glBindTexture(GL_TEXTURE_2D, cloud_texture.get_vto());

//         glActiveTexture(GL_TEXTURE0);
//         glBindTexture(GL_TEXTURE_2D, earth_texture.get_vto());

//         glDrawArrays(GL_TRIANGLES, 0, mesh.get_vertex_count());
//         glBindTexture(GL_TEXTURE_2D, 0);

//         // Moon
//         // set programme
//         // moonProgram.m_Program.use();
//         blinnPhongProgram.m_Program.use();
//         // set the moon texture on the unit 1 and bind it
//         glUniform1i(blinnPhongProgram.uTexture, 0);
//         glActiveTexture(GL_TEXTURE0);
//         glBindTexture(GL_TEXTURE_2D, moon_texture.get_vto());

//         for (unsigned int i = 0; i < list_axis.size() - 1; i++)
//         {
//             auto axis      = list_axis[i];
//             auto start_pos = list_pos[i];

//             glm::mat4 mat = get_transformations(start_time, MVMatrix, axis, start_pos);

//             blinnPhongProgram.passLight(list_light, list_dir_light, MVMatrix);
//             blinnPhongProgram.passMaterial(material);
//             blinnPhongProgram.passMatrix(mat, ProjMatrix);

//             glBindVertexArray(mesh.get_vao());

//             glActiveTexture(GL_TEXTURE0);
//             glBindTexture(GL_TEXTURE_2D, moon_texture.get_vto());

//             glDrawArrays(GL_TRIANGLES, 0, mesh.get_vertex_count());
//             glBindTexture(GL_TEXTURE_2D, 0);
//         }

//         glBindVertexArray(0);

//         /* Swap front and back buffers */
//         glfwSwapBuffers(window);
//         /* Poll for and process events */
//         glfwPollEvents();

//         // GET THE DELTAS
//         float end_time = glfwGetTime();
//         delta_time     = end_time - start_time;

//         glfwGetCursorPos(window, &mousePosEnd.x, &mousePosEnd.y);

//         mouse.delta = mousePosEnd - mousePosStart;
//     }
//     // glDeleteBuffers(1, get_vao_ptr());
//     // glDeleteVertexArrays(1, get_vbo_ptr());
//     glfwTerminate();
//     return 0;
// }

// void debug_movement_input(MovementInput input)
// {
//     std::cout << "Up : " << (char)input.up_key << " / " << input.up_pressed << std::endl;
//     std::cout << "down : " << (char)input.down_key << " / " << input.down_pressed << std::endl;
//     std::cout << "left : " << (char)input.left_key << " / " << input.left_pressed << std::endl;
//     std::cout << "right : " << (char)input.right_key << " / " << input.right_pressed << std::endl;
// }