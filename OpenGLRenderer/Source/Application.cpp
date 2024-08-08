#include <OpenGLRenderer/Application.hpp>

#include <OpenGLRenderer/Core/Logger.hpp>

namespace PixL {
    Application* Application::m_SApplicationInstance = nullptr;

    Application::Application(const WindowProperties& properties) {
        if (m_SApplicationInstance) {
            Log::EngineFatal({0x00, 0x01}, "Only one instance of this application can run at once.");
        }
        
        m_SApplicationInstance = this;

        Logger::Init();

        m_Window = std::make_unique<Window>(properties);
        m_Window->SetEventCallback([this](Event& event) {
            EventDispatcher dispatcher(event);
            dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_TO_EVENT_HANDLER(Application::OnWindowClose));
            OnEvent(event);
        });

        Log::AppInfo("Launching application...");

        m_IsRunning = true;
    }

    Application::~Application() {
        Log::AppInfo("Quitting application...");
        m_SApplicationInstance = nullptr;
    }
    
    void Application::Run() {
        while (m_IsRunning) {
            m_Window->Update();
            OnUpdate();
            OnRender();
        };
    }

    void Application::OnWindowClose(WindowCloseEvent& event) {
        m_IsRunning = false;
    }
}
