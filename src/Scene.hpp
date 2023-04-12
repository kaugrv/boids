#pragma once

#include <memory>
#include <vector>
#include "Sdf.hpp"
#include "p6/p6.h"

class Scene {
private:
    std::vector<std::unique_ptr<Obstacle>> m_Obstacles{};

public:
    Scene()  = default;
    ~Scene() = default;

    void draw(p6::Context& ctx)
    {
        for (auto& obstacle : m_Obstacles)
        {
            obstacle->draw(ctx);
        }
    }

    auto get_obstacles() const
    {
        return &m_Obstacles;
    }

    void add_obstacle(Obstacle* obstacle_ptr)
    {
        if (obstacle_ptr != nullptr)
            m_Obstacles.push_back(std::unique_ptr<Obstacle>(obstacle_ptr));
    }

    void remove_obstacle()
    {
        m_Obstacles.pop_back();
    }
};
