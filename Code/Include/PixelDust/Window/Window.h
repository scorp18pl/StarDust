#pragma once

#include <cstddef>
#include <cstdint>
#include <GLFW/glfw3.h>

namespace Pxd
{
    class Window
    {
    public:
        enum Flags : uint8_t
        {
            None = 0,
        };

        Window(
            int width,
            int height, const char* title,
            int pixelCountX = 1LU,
            int pixelCountY = 1LU,
            Window::Flags = Flags::None);
        Window(const Window&) = delete;
        Window(Window&&) = delete;
        ~Window();



        size_t GetWidth() const;
        size_t GetHeight() const;

        Window& operator=(const Window&) = delete;

    private:
        GLFWwindow* m_window;
        int m_width, m_height;
    };

} // namespace Pxd
