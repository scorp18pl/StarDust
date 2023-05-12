#pragma once

#include <StarDust/Renderer.h>
#include <cstddef>
#include <cstdint>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace Str
{
    class Window
    {
    public:
        Window(int width, int height, const char* title);
        Window(const Window&) = delete;
        Window(Window&&) = delete;
        ~Window();

        [[nodiscard]] bool IsOpen() const;
        [[nodiscard]] int GetWidth() const;
        [[nodiscard]] int GetHeight() const;

        void Clear();
        void Update() const;

        void SetPixelizationFactor(unsigned int factor);
        GLFWwindow* GetNativeWindow() const;

        Window& operator=(const Window&) = delete;

    private:
        GLFWwindow* m_window;
        int m_width, m_height;
    };

} // namespace Str
