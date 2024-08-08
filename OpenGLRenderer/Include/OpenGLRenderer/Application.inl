#pragma once

inline Application& Application::GetInstance() {
    return *m_SApplicationInstance;
}