#include "enginepch.h"
#include "WindowsWindow.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Engine/Events/ApplicationEvent.h"
#include "Engine/Events/KeyEvent.h"
#include "Engine/Events/MouseEvent.h"

#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/GraphicsContext.h"

namespace Engine
{
    static uint8_t s_GLFWWindowCount{ 0 };

#if defined ENGINE_DEBUG
    static void GLFWErrorCallback(int error, const char* description)
    {
        ENGINE_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
    }
#endif

    WindowsWindow::WindowsWindow(const WindowProps& props)
        : m_pWindow{}
        , m_Data{}
    {
        ENGINE_PROFILE_FUNCTION();

        Init(props);
    }

    WindowsWindow::~WindowsWindow()
    {
        Shutdown();
    }

    void WindowsWindow::OnUpdate()
    {
        ENGINE_PROFILE_FUNCTION();
        {
            ENGINE_PROFILE_SCOPE("WindowsWindow::OnUpdate-glfwPollEvents");
            glfwPollEvents();
        }
        m_Context->SwapBuffers();
    }

    void WindowsWindow::SetPosition(uint32_t x, uint32_t y)
    {
		m_Data.x = x;
		m_Data.y = y;
        glfwSetWindowPos(m_pWindow, static_cast<uint32_t>(x), static_cast<uint32_t>(y));
    }

    uint32_t WindowsWindow::GetPositionX() const
    {
        return m_Data.x;
    }

    uint32_t WindowsWindow::GetPositionY() const
    {
        return m_Data.y;
    }

    void WindowsWindow::Minimize()
    {
        glfwIconifyWindow(m_pWindow);
    }

    void WindowsWindow::Maximize()
    {
        glfwMaximizeWindow(m_pWindow);
    }

    void WindowsWindow::Restore()
    {
        glfwRestoreWindow(m_pWindow);
    }

    bool WindowsWindow::IsMinimized() const
    {
        return glfwGetWindowAttrib(m_pWindow, GLFW_ICONIFIED) == GLFW_TRUE;
    }

    bool WindowsWindow::IsMaximized() const
    {
        return glfwGetWindowAttrib(m_pWindow, GLFW_MAXIMIZED) == GLFW_TRUE;
    }

    bool WindowsWindow::IsFocused() const
    {
        return glfwGetWindowAttrib(m_pWindow, GLFW_FOCUSED) == GLFW_TRUE;
    }

    bool WindowsWindow::IsHovered() const
    {
        return glfwGetWindowAttrib(m_pWindow, GLFW_HOVERED) == GLFW_TRUE;
    }

    void WindowsWindow::SetResizable(bool resizable)
    {
        glfwSetWindowAttrib(m_pWindow, GLFW_RESIZABLE, resizable ? GLFW_TRUE : GLFW_FALSE);
    }

    void WindowsWindow::SetSize(uint32_t width, uint32_t height)
    {
        m_Data.width = width;
        m_Data.height = height;
        glfwSetWindowSize(m_pWindow, static_cast<int>(width), static_cast<int>(height));
    }

    uint32_t WindowsWindow::GetWidth() const
    {
        return m_Data.width;
    }

    uint32_t WindowsWindow::GetHeight() const
    {
        return m_Data.height;
    }

    void WindowsWindow::SetSizeLimits(std::optional<uint32_t> minWidth, std::optional<uint32_t> minHeight, std::optional<uint32_t> maxWidth, std::optional<uint32_t> maxHeight)
    {
        const int minW{ minWidth.has_value() ? static_cast<int>(*minWidth) : GLFW_DONT_CARE };
        const int minH{ minHeight.has_value() ? static_cast<int>(*minHeight) : GLFW_DONT_CARE };
        const int maxW{ maxWidth.has_value() ? static_cast<int>(*maxWidth) : GLFW_DONT_CARE };
        const int maxH{ maxHeight.has_value() ? static_cast<int>(*maxHeight) : GLFW_DONT_CARE };

        glfwSetWindowSizeLimits(m_pWindow, minW, minH, maxW, maxH);
    }

    void WindowsWindow::SetFullscreen(bool fullscreen)
    {
        if (m_Data.fullScreen == fullscreen) return;
        m_Data.fullScreen = fullscreen;

        GLFWmonitor* const monitor{ glfwGetPrimaryMonitor() };
        ENGINE_CORE_ASSERT_MSG(monitor, "Failed to get primary monitor!");

        const GLFWvidmode* const videoMode{ glfwGetVideoMode(monitor) };
        ENGINE_CORE_ASSERT_MSG(videoMode, "Failed to get video mode!");

        if (!monitor || !videoMode) return;

        if (fullscreen)
        {
            m_Data.x = 0;
            m_Data.y = 0;

            // Save last windowed position and size
            glfwGetWindowPos(m_pWindow, &m_LastWindowedPosX, &m_LastWindowedPosY);
            glfwGetWindowSize(m_pWindow, &m_LastWindowedSizeX, &m_LastWindowedSizeY);

            glfwSetWindowMonitor(m_pWindow, monitor, 0, 0, videoMode->width, videoMode->height, videoMode->refreshRate);
        }
        else
        {
            Restore();
            // Restore windowed size & position
            glfwSetWindowMonitor(m_pWindow, nullptr, m_LastWindowedPosX, m_LastWindowedPosY, m_LastWindowedSizeX, m_LastWindowedSizeY, videoMode->refreshRate);
        }
    }

    bool WindowsWindow::IsFullscreen() const
    {
        return m_Data.fullScreen;
    }

    void WindowsWindow::SetVSync(bool enabled)
    {
        if (enabled) glfwSwapInterval(1);
        else glfwSwapInterval(0);
        m_Data.vSync = enabled;
    }

    bool WindowsWindow::IsVSync() const
    {
        return m_Data.vSync;
    }

    void WindowsWindow::SetEventCallback(const EventCallbackFn& callback)
    {
        m_Data.eventCallback = callback;
    }

    void* WindowsWindow::GetNativeWindow() const
    {
        return m_pWindow;
    }

    void WindowsWindow::Init(const WindowProps& props)
    {
        m_Data.title = props.title;
        m_Data.width = props.width;
        m_Data.height = props.height;

        if (s_GLFWWindowCount == 0)
        {
            {
                ENGINE_PROFILE_SCOPE("WindowsWindow::Init-glfwInit");
                const int success{ glfwInit() };
                ENGINE_CORE_ASSERT_MSG(success, "Could not initialize GLFW!");
            }

            glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_FALSE);

#if defined ENGINE_DEBUG

            glfwSetErrorCallback(GLFWErrorCallback);
            ENGINE_CORE_INFO("Created window {0} ({1}, {2})", props.title, props.width, props.height);

            if (Renderer::GetAPI() == RendererAPI::API::OpenGL)
            {
                glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
                glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
                glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

                glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
                glfwWindowHint(GLFW_DEPTH_BITS, 24);
            }
#endif
        }

        {
            ENGINE_PROFILE_SCOPE("WindowsWindow::Init-glfwCreateWindow");

            GLFWmonitor* const monitor{ glfwGetPrimaryMonitor() };
            ENGINE_CORE_ASSERT_MSG(monitor, "Failed to get primary monitor!");

            const GLFWvidmode* const videoMode{ glfwGetVideoMode(monitor) };
            ENGINE_CORE_ASSERT_MSG(videoMode, "Failed to get video mode!");

            if (props.fullScreen && monitor && videoMode)
            {
                // If starting fullscreen, prepare reasonable restore values for when we exit fullscreen
                const int centerX{ (videoMode->width - static_cast<int>(m_Data.width)) / 2 };
                const int centerY{ (videoMode->height - static_cast<int>(m_Data.height)) / 2 };
                m_LastWindowedPosX = centerX;
                m_LastWindowedPosY = centerY;
                m_LastWindowedSizeX = static_cast<int>(m_Data.width);
                m_LastWindowedSizeY = static_cast<int>(m_Data.height);

                //VideoMode width and height is the actual size of you monitor
                m_Data.fullScreen = props.fullScreen;
                m_Data.width = static_cast<uint32_t>(videoMode->width);
                m_Data.height = static_cast<uint32_t>(videoMode->height);
            }

            m_pWindow = glfwCreateWindow
            (
                static_cast<int>(m_Data.width),
                static_cast<int>(m_Data.height),
                m_Data.title.c_str(),
                m_Data.fullScreen ? monitor : nullptr,
                nullptr
            );

            ENGINE_CORE_ASSERT_MSG(m_pWindow, "Failed to create GLFW window!");

            // Center Window
            if (!props.fullScreen)
            {
                int windowWidth{};
                int windowHeight{};
                glfwGetWindowSize(m_pWindow, &windowWidth, &windowHeight);

                const int posX{ (videoMode->width - windowWidth) / 2 };
                const int posY{ (videoMode->height - windowHeight) / 2 };
                glfwSetWindowPos(m_pWindow, posX, posY);
            }
        }
        ++s_GLFWWindowCount;

        // Context
        m_Context = GraphicsContext::Create(m_pWindow);
        m_Context->Init();

        glfwSetWindowUserPointer(m_pWindow, &m_Data);

        SetVSync(props.vsync);

        InitCallbacks();
    }

    void WindowsWindow::InitCallbacks()
    {
        // Set GLFW callbacks
        glfwSetWindowSizeCallback(m_pWindow, [](GLFWwindow* window, int width, int height)
        {
            WindowData& data{ *(WindowData*)glfwGetWindowUserPointer(window) };
            data.width = static_cast<uint32_t>(width);
            data.height = static_cast<uint32_t>(height);

            WindowResizeEvent event{ data.width, data.height };
            data.eventCallback(event);
        });

        glfwSetWindowCloseCallback(m_pWindow, [](GLFWwindow* window)
        {
            const WindowData& data{ *(WindowData*)glfwGetWindowUserPointer(window) };
            WindowCloseEvent event{};
            data.eventCallback(event);

        });

        glfwSetKeyCallback(m_pWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            const WindowData& data{ *(WindowData*)glfwGetWindowUserPointer(window) };

            switch (action)
            {
            case GLFW_PRESS:
            {
                KeyPressedEvent event{ static_cast<unsigned short>(key), 0 };
                data.eventCallback(event);
                break;
            }
            case GLFW_RELEASE:
            {
                KeyReleasedEvent event{ static_cast<unsigned short>(key) };
                data.eventCallback(event);
                break;
            }
            case GLFW_REPEAT:
            {
                KeyPressedEvent event{ static_cast<unsigned short>(key), true };
                data.eventCallback(event);
                break;
            }
            }
        });

        glfwSetCharCallback(m_pWindow, [](GLFWwindow* window, unsigned int keycode)
        {
            const WindowData& data{ *(WindowData*)glfwGetWindowUserPointer(window) };

            KeyTypedEvent event{ static_cast<unsigned short>(keycode) };
            data.eventCallback(event);
        });

        glfwSetMouseButtonCallback(m_pWindow, [](GLFWwindow* window, int button, int action, int mods)
        {
            const WindowData& data{ *(WindowData*)glfwGetWindowUserPointer(window) };

            switch (action)
            {
            case GLFW_PRESS:
            {
                MouseButtonPressedEvent event{ static_cast<unsigned short>(button) };
                data.eventCallback(event);
                break;
            }
            case GLFW_RELEASE:
            {
                MouseButtonReleasedEvent event{ static_cast<unsigned short>(button) };
                data.eventCallback(event);
                break;
            }
            }
        });

        glfwSetScrollCallback(m_pWindow, [](GLFWwindow* window, double xOffset, double yOffset)
        {
            const WindowData& data{ *(WindowData*)glfwGetWindowUserPointer(window) };

            MouseScrolledEvent event{ static_cast<float>(xOffset), static_cast<float>(yOffset) };
            data.eventCallback(event);
        });

        glfwSetCursorPosCallback(m_pWindow, [](GLFWwindow* window, double xPos, double yPos)
        {
            const WindowData& data{ *(WindowData*)glfwGetWindowUserPointer(window) };

            MouseMovedEvent event{ static_cast<float>(xPos), static_cast<float>(yPos) };
            data.eventCallback(event);
        });
    }

    void WindowsWindow::Shutdown()
    {
        ENGINE_PROFILE_FUNCTION();

        glfwDestroyWindow(m_pWindow);
        --s_GLFWWindowCount;

        if (s_GLFWWindowCount == 0)
        {
            glfwTerminate();
        }
    }
}