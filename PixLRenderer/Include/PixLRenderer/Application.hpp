#pragma once

#include <PixLRenderer/Core/Event.hpp>
#include <PixLRenderer/Core/Window.hpp>

#include <PixLRenderer/pch.hpp>

namespace PixL {
    class Application {
        static Application* m_SApplicationInstance;

        bool m_IsRunning = false;
        
    public:
        explicit Application(const WindowProperties& properties);
        virtual ~Application();

        Application(const Application&) = delete;
        Application(Application&&) = delete;

        Application& operator=(const Application&) = delete;
        Application& operator=(Application&&) = delete;

        void Run();

        [[nodiscard]] static inline Application& GetInstance();
        
    protected:
        std::unique_ptr<Window> m_Window = nullptr;

        virtual void OnEvent(Event& event) = 0;
        virtual void OnUpdate() = 0;
        virtual void OnRender() = 0;

        void OnWindowClose(WindowCloseEvent& event);

        inline void Quit();
    };

#include <PixLRenderer/Application.inl>
}
