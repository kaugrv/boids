// #pragma once

// #include "glimac/FilePath.hpp"
// #include "glimac/Image.hpp"
// #include "glimac/Sphere.hpp"
// #define GLFW_INCLUDE_NONE
// #include <GLFW/glfw3.h>
// #include <glad/glad.h>
// #include <glimac/glm.hpp>

// void fill_vto(const std::unique_ptr<glimac::Image>& img_ptr, GLuint& vto)
// {
//     glGenTextures(1, &vto);
//     glBindTexture(GL_TEXTURE_2D, vto);
//     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_ptr.get()->getWidth(), img_ptr.get()->getHeight(), 0, GL_RGBA, GL_FLOAT, img_ptr.get()->getPixels());
// }

// class Texture {
// private:
//     std::unique_ptr<glimac::Image> m_img_ptr;

//     GLuint m_vto;

// public:
//     Texture() = default;

//     ~Texture()
//     {
//     }

//     // TODO constructor from a file path (.obj)
//     Texture(const glimac::FilePath& path)
//     {
//         m_img_ptr = glimac::loadImage(path);
//         if (m_img_ptr == nullptr)
//             std::cout << "null"; // bad line, will be fixed with p6

//         fill_vto(m_img_ptr, m_vto);

//         // parameters of the texture
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

//         glBindTexture(GL_TEXTURE_2D, 0);
//     }

//     GLuint get_vto()
//     {
//         return m_vto;
//     }

//     glm::vec4* get_img() const
//     {
//         return m_img_ptr->getPixels();
//     }

//     unsigned int get_width() const
//     {
//         return m_img_ptr->getWidth();
//     }

//     unsigned int get_height() const
//     {
//         return m_img_ptr->getHeight();
//     }
// };
