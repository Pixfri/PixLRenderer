#pragma once

inline Application& Application::GetInstance() {
    return *m_SApplicationInstance;
}

inline void Application::Quit() {
    m_IsRunning = false;
}
