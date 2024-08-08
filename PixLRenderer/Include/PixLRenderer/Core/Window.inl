#pragma once

inline GLFWwindow* Window::GetWindow() const {
    return m_Window;
}

inline i32 Window::GetWidth() const {
    return m_Data.Width;
}

inline i32 Window::GetHeight() const {
    return m_Data.Height;
}

inline bool Window::IsFocused() const {
    return m_Data.Focused;
}

inline bool Window::ShouldClose() const {
    return glfwWindowShouldClose(m_Window);
}

inline bool Window::IsFullscreen() const {
    return m_Data.Fullscreen;
}

inline bool Window::IsVSyncEnabled() const {
    return m_Data.VSyncEnabled;
}

inline void Window::Update() const {
    glfwPollEvents();
    glfwSwapBuffers(m_Window);
}

inline void Window::Close() const {
    glfwSetWindowShouldClose(m_Window, true);
}

inline void Window::SetEventCallback(const std::function<void(Event&)>& callback) {
    m_Data.EventCallback = callback;
}

inline void Window::SetVSync(const bool enabled) {
    glfwSwapInterval(enabled);
    m_Data.VSyncEnabled = enabled;
}

inline void Window::SetFullscreen(const bool enabled) {
    m_Data.ShouldUpdateFullscreen = true;
    m_Data.Fullscreen = enabled;
    UpdateFullscreenMode();
}