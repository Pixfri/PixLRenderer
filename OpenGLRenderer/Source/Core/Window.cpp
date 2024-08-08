#include <OpenGLRenderer/Core/Window.hpp>

#include <OpenGLRenderer/Core/Logger.hpp>

namespace OpenGLRenderer {
    Window::Window(const WindowProperties& properties) {
        if (!glfwInit()) {
            Log::EngineFatal({0x01, 0x00}, "Failed to initialize GLFW.");
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#if defined(__APPLE__)
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif

        m_Window = glfwCreateWindow(800, 600, "OpenGL Renderer", nullptr, nullptr);
        if (m_Window == nullptr) {
            glfwTerminate();
            Log::EngineFatal({0x01, 0x01}, "Failed to create GLFW window.");
        }
        glfwMakeContextCurrent(m_Window);

        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
            // NOLINT(clang-diagnostic-cast-function-type-strict)
            Log::EngineFatal({0x01, 0x02}, "Failed to initialize GLAD.");
        }

        m_Data.Width = properties.Width;
        m_Data.Height = properties.Height;
        m_Data.Focused = true;

        glfwSetWindowUserPointer(m_Window, &m_Data);


        // ------------------------------- Window callbacks ------------------------------- //
        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
            const auto data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            WindowCloseEvent event;
            data->EventCallback(event);
        });

        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, const i32 width, const i32 height) {
            const auto data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            glViewport(0, 0, width, height);
            data->Width = width;
            data->Height = height;
            WindowResizeEvent event(width, height);
            data->EventCallback(event);
        });

        glfwSetWindowPosCallback(m_Window, [](GLFWwindow* window, const i32 x, const i32 y) {
            const auto data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            WindowMovedEvent event(x, y);
            data->EventCallback(event);
        });

        glfwSetKeyCallback(m_Window, [](GLFWwindow* window, const i32 key, [[maybe_unused]] const i32 scancode,
                                        const i32 action,
                                        [[maybe_unused]] const i32 mods) {
            const auto data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            switch (action) {
            case GLFW_PRESS:
                {
                    KeyDownEvent event(key, 0);
                    data->EventCallback(event);
                    break;
                }
            case GLFW_RELEASE:
                {
                    KeyUpEvent event(key);
                    data->EventCallback(event);
                    break;
                }
            case GLFW_REPEAT:
                {
                    KeyDownEvent event(key, 1);
                    data->EventCallback(event);
                    break;
                }
            default:
                Log::EngineError("Unknown GLFW key action.");
            }
        });

        glfwSetCharCallback(m_Window, [](GLFWwindow* window, const u32 keycode) {
            const auto data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            KeyTypedEvent event(static_cast<i32>(keycode));
            data->EventCallback(event);
        });

        glfwSetMouseButtonCallback(
            m_Window, [](GLFWwindow* window, const i32 button, const i32 action, [[maybe_unused]] const i32 mods) {
                const auto data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));

                switch (action) {
                case GLFW_PRESS:
                    {
                        MouseButtonDownEvent event(button);
                        data->EventCallback(event);
                        break;
                    }
                case GLFW_RELEASE:
                    {
                        MouseButtonUpEvent event(button);
                        data->EventCallback(event);
                        break;
                    }
                default:
                    Log::EngineError("Unknown GLFW mouse button action.");
                }
            });

        glfwSetScrollCallback(m_Window, [](GLFWwindow* window, const f64 xOffset, const f64 yOffset) {
            const auto data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            MouseScrolledEvent event(static_cast<f32>(xOffset), static_cast<f32>(yOffset));
            data->EventCallback(event);
        });

        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, const f64 xPos, const f64 yPos) {
            const auto data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            MouseMovedEvent event(static_cast<f32>(xPos), static_cast<f32>(yPos));
            data->EventCallback(event);
        });

        glfwSetWindowFocusCallback(m_Window, [](GLFWwindow* window, const i32 focused) {
            const auto data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            WindowFocusedEvent event(focused);
            data->EventCallback(event);
            data->Focused = focused;
        });

        m_Initialized = true;
    }

    Window::~Window() {
        if (m_Initialized) {
            glfwDestroyWindow(m_Window);
        }

        glfwTerminate();
    }
}
