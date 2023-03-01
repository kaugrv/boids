#include <cstdlib>
#include <vector>
#include "p6/p6.h"
#define DOCTEST_CONFIG_IMPLEMENT
#include <imgui.h>
#include "Boid.hpp"
#include "doctest/doctest.h"

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

    // List of Boids
    std::vector<Boid> boids(100);
    for (int i = 0; i < 100; i++)
    {
        boids[i] = Boid(glm::vec2(p6::random::number(-1., 1.), p6::random::number(-1., 1.)), p6::random::number(-1., 1.), p6::random::number(0., 2 * p6::PI), 0.1);
    }

    ctx.update = [&]() {
        ctx.background(p6::NamedColor::Cyan);

        for (auto& boid : boids)
        {
            boid.update_position(ctx.delta_time());
            boid.draw(ctx);
        }
    };

    ctx.maximize_window();

    // Should be done last. It starts the infinite loop.
    ctx.start();
};