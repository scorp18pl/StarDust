#include <PixelDust/Window/Window.h>

#include <glad/glad.h>
#include <stdexcept>

namespace Pxd
{
    Window::Window(
        int width,
        int height,
        const char* title,
        int pixelCountX,
        int pixelCountY,
        Window::Flags)
        : m_width{ width }
        , m_height{ height }
    {
        if (!glfwInit())
        {
            throw std::runtime_error("Failed to initialize GLFW.");
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
        m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
        if (!m_window)
        {
            glfwTerminate();
            throw std::runtime_error("Failed to create window.");
        }
    }

    Window::~Window()
    {
        if (m_window)
        {
            glfwDestroyWindow(m_window);
            glfwTerminate();
        }
    }

    size_t Window::GetWidth() const
    {
        return m_width;
    }

    size_t Window::GetHeight() const
    {
        return m_height;
    }
} // namespace Pxd