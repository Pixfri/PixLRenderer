#pragma once

#include <OpenGLRenderer/Core/Event.hpp>

#include <OpenGLRenderer/pch.hpp>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <functional>

namespace OpenGLRenderer {
    struct WindowProperties {
        i32 Width, Height;
        std::string Title;
        bool Fullscreen;
        bool VSync;

        WindowProperties(const i32 width, const i32 height, std::string&& title, const bool fullscreen,
                         const bool vsync) : Width(width), Height(height), Title(std::move(title)),
                                             Fullscreen(fullscreen), VSync(vsync) {
        }
    };

    struct WindowData {
        i32 Width, Height;
        bool Focused;
        std::function<void(Event&)> EventCallback;
    };

    class Window {
        GLFWwindow* m_Window;
        WindowData m_Data;

        bool m_Initialized = false;
    
    public:
        explicit Window(const WindowProperties& properties);
        ~Window();

        Window(const Window&) = delete;
        Window(Window&&) = delete;

        Window& operator=(const Window&) = delete;
        Window& operator=(Window&&) = delete;

        [[nodiscard]] inline GLFWwindow* GetWindow() const;
        [[nodiscard]] inline i32 GetWidth() const;
        [[nodiscard]] inline i32 GetHeight() const;
        [[nodiscard]] inline bool IsFocused() const;
        [[nodiscard]] inline bool ShouldClose() const;

        inline void Update() const;
        inline void Close() const;
        inline void SetEventCallback(const std::function<void(Event&)>& callback);
    };

#include <OpenGLRenderer/Core/Window.inl>
}