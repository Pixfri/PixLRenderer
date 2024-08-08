#include <PixLRenderer/Core/Window.hpp>

#include <PixLRenderer/Core/Logger.hpp>

namespace PixL {
    Window::Window(const WindowProperties& properties) {
        if (!glfwInit()) {
            Log::EngineFatal({0x01, 0x00}, "Failed to initialize GLFW.");
        }

        Log::EngineTrace("Creating window and initializing OpenGL loader...");

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#if defined(__APPLE__)
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif

        const GLFWvidmode* videoMode = glfwGetVideoMode(glfwGetPrimaryMonitor());

        const i32 width = properties.Fullscreen ? videoMode->width : properties.Width;
        const i32 height = properties.Fullscreen ? videoMode->height : properties.Height;

        m_Window = glfwCreateWindow(width, height, properties.Title.c_str(),
                                    properties.VSync ? glfwGetPrimaryMonitor() : nullptr, nullptr);
        if (m_Window == nullptr) {
            glfwTerminate();
            Log::EngineFatal({0x01, 0x01}, "Failed to create GLFW window.");
        }
        glfwMakeContextCurrent(m_Window);

        if (!properties.Fullscreen) {
            const i32 windowLeft = videoMode->width / 2 - properties.Width / 2;
            const i32 windowTop = videoMode->height / 2 - properties.Height / 2;
            glfwSetWindowPos(m_Window, windowLeft, windowTop);
        }

        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
            // NOLINT(clang-diagnostic-cast-function-type-strict)
            Log::EngineFatal({0x01, 0x02}, "Failed to initialize GLAD.");
        }

        Log::EngineTrace("Window created and OpenGL loader initialized. Setting up callbacks...");

        m_Data.Width = properties.Width;
        m_Data.Height = properties.Height;
        m_Data.Focused = true;
        m_Data.Fullscreen = properties.Fullscreen;
        m_Data.VSyncEnabled = properties.VSync;
        m_Data.ShouldUpdateFullscreen = false;

        glfwSetWindowUserPointer(m_Window, &m_Data);

        SetVSync(properties.VSync);

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

        Log::EngineTrace("Callbacks are set up successfully.");

        m_Initialized = true;
    }

    Window::~Window() {
        if (m_Initialized) {
            Log::EngineTrace("Destroying window.");
            glfwDestroyWindow(m_Window);
        }

        glfwTerminate();
    }

    void Window::UpdateFullscreenMode() {
        if (m_Data.ShouldUpdateFullscreen) {
            static i32 posX, posY;
            glfwGetWindowPos(m_Window, &posX, &posY);
            static i32 sizeX, sizeY;
            glfwGetWindowSize(m_Window, &sizeX, &sizeY);

            if (m_Data.Fullscreen) {
                const auto monitor = glfwGetPrimaryMonitor();
                const auto mode = glfwGetVideoMode(monitor);

                // store position and size to restore later
                glfwGetWindowPos(m_Window, &posX, &posY);
                glfwGetWindowSize(m_Window, &sizeX, &sizeY);

                glfwSetWindowMonitor(m_Window, monitor, 0, 0, mode->width, mode->height,
                                     IsVSyncEnabled() ? mode->refreshRate : 0);
            } else {
                glfwSetWindowMonitor(m_Window, nullptr, posX, posY, sizeX, sizeY, 0);
            }
            m_Data.ShouldUpdateFullscreen = false;
        }
    }
}
