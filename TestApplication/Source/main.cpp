#include <TestApplication.hpp>

int main() {
    OpenGLRenderer::WindowProperties properties{800, 600, "Test Application", false, false};
    const auto testApplication = std::make_unique<TestApplication>(properties);

    testApplication->Run();

    return EXIT_SUCCESS;
}