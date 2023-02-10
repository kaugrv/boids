#include <cstdlib>
#include <vector>
#include "p6/p6.h"
#define DOCTEST_CONFIG_IMPLEMENT
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

    auto ctx = p6::Context{{.title = "Explosion !!"}};

    // List of square starting centers
    std::vector<float> centers(200);
    for (int i = 0; i < 200; i += 2)
    {
        centers[i]     = p6::random::number(-ctx.aspect_ratio(), ctx.aspect_ratio()); // x
        centers[i + 1] = p6::random::number(-1.0f, 1.0f);
    }

    // Declare your infinite update loop.
    ctx.update = [&]() {
        ctx.background(p6::NamedColor::Cyan);
        for (int i = 0; i < 100; i++)
        {
            p6::Color fill{1.f, 0.f, 0.f, 0.5f};
            ctx.square(
                p6::Center{
                    centers[i] * ctx.time(),
                    centers[i + 1] * ctx.time(),
                },
                p6::Radius{0.1f}
            );
        }
    };

    ctx.maximize_window();

    // Should be done last. It starts the infinite loop.
    ctx.start();
}