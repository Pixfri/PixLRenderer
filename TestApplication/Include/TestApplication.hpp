#pragma once

#include <OpenGLRenderer/Application.hpp>

class TestApplication final : public PixL::Application {
public:
    TestApplication(const PixL::WindowProperties& properties);
    ~TestApplication() override = default;

    TestApplication(const TestApplication&) = delete;
    TestApplication(TestApplication&&) = delete;

    TestApplication& operator=(const TestApplication&) = delete;
    TestApplication& operator=(TestApplication&&) = delete;

protected:
    void OnEvent(PixL::Event& event) override;
    void OnUpdate() override;
    void OnRender() override;

    void OnInput(const PixL::KeyDownEvent& event);

    bool m_IsFullscreen = false;
};
