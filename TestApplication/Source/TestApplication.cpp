#include <TestApplication.hpp>

#include <PixLRenderer/Core/Logger.hpp>

TestApplication::TestApplication(const PixL::WindowProperties& properties) : Application(properties) {
}


void TestApplication::OnEvent(PixL::Event& event) {
    PixL::EventDispatcher dispatcher(event);

    dispatcher.Dispatch<PixL::KeyDownEvent>(BIND_EVENT_TO_EVENT_HANDLER(TestApplication::OnInput));
}

void TestApplication::OnUpdate() {
}

void TestApplication::OnRender() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void TestApplication::OnInput(const PixL::KeyDownEvent& event) {
    if (event.GetScancode() == static_cast<i32>(PixL::Keys::Escape)) {
        Quit();
    }
}