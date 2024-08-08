#include <TestApplication.hpp>

int main() {
    PixL::WindowProperties properties{800, 600, "PixL Test Application", false, false};
    const auto testApplication = std::make_unique<TestApplication>(properties);

    testApplication->Run();

    return EXIT_SUCCESS;
}