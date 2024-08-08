#pragma once

#include <OpenGLRenderer/Application.hpp>

class TestApplication final : public OpenGLRenderer::Application {
public:
    TestApplication(const OpenGLRenderer::WindowProperties& properties);
    ~TestApplication() override = default;

    TestApplication(const TestApplication&) = delete;
    TestApplication(TestApplication&&) = delete;

    TestApplication& operator=(const TestApplication&) = delete;
    TestApplication& operator=(TestApplication&&) = delete;

protected:
    void OnEvent(OpenGLRenderer::Event& event) override;
    void OnUpdate() override;
    void OnRender() override;

    void OnInput(OpenGLRenderer::KeyDownEvent& event);

    bool m_IsFullscreen = false;
};
