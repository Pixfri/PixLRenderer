#include <TestApplication.hpp>

#include <OpenGLRenderer/Core/Logger.hpp>

TestApplication::TestApplication(const OpenGLRenderer::WindowProperties& properties) : Application(properties) {
}


void TestApplication::OnEvent(OpenGLRenderer::Event& event) {
    OpenGLRenderer::EventDispatcher dispatcher(event);

    dispatcher.Dispatch<OpenGLRenderer::KeyDownEvent>(BIND_EVENT_TO_EVENT_HANDLER(TestApplication::OnInput));
}

void TestApplication::OnUpdate() {
}

void TestApplication::OnRender() {
}

void TestApplication::OnInput(OpenGLRenderer::KeyDownEvent& event) {
    
}