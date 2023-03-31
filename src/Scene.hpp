#pragma once

#include "Sdf.hpp"
#include <iostream>
#include <memory>
#include <vector>
#include "p6/p6.h"

class Scene {
    private:
        std::vector<std::unique_ptr<Shape>> m_Shapes;

    public:
        Scene() = default;
        ~Scene() = default;

        void draw(p6::Context& ctx) {
            for (auto& shape: m_Shapes) {
                shape->draw(ctx);
            }
        }

        auto get_shapes() const {
            
            return &m_Shapes;
        }

        void add_shape(Shape* shape_ptr) {
            if(shape_ptr != nullptr)
                m_Shapes.push_back(std::unique_ptr<Shape>(shape_ptr));
        }

        void remove_shape() {
            m_Shapes.pop_back();
        }

};

