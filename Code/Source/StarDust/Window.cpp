#include <StarDust/Drawable/Drawable.h>
#include <StarDust/Utils.h>
#include <StarDust/Window.h>
#include <glad/glad.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <stdexcept>

namespace Str
{
    Window::Window(int width, int height, const char* title)
        : m_width{ width }
        , m_height{ height }
    {
        if (!glfwInit())
        {
            throw std::runtime_error("Failed to initialize GLFW.");
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
        if (!m_window)
        {
            glfwTerminate();
            throw std::runtime_error("Failed to create window.");
        }

        glfwMakeContextCurrent(m_window);

        if (!gladLoadGL())
        {
            glfwTerminate();
            throw std::runtime_error("Could not initialize glad.");
        }

        glfwSwapInterval(0);
        glEnable(GL_DEPTH_TEST);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        (void)io;

        ImGui_ImplGlfw_InitForOpenGL(m_window, true);
        ImGui_ImplOpenGL3_Init("#version 460");

        ImGui::StyleColorsDark();
    }

    Window::~Window()
    {
        if (m_window)
        {
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();

            glfwDestroyWindow(m_window);
            glfwTerminate();
        }
    }

    bool Window::IsOpen() const
    {
        return !glfwWindowShouldClose(m_window);
    }

    int Window::GetWidth() const
    {
        return m_width;
    }

    int Window::GetHeight() const
    {
        return m_height;
    }

    void Window::Clear()
    {
        m_renderer.RenderBatched();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        GL_CHECK(glfwSwapBuffers(m_window));
        GL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    }

    void Window::Update() const
    {
        GL_CHECK(glfwPollEvents());

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void Window::Draw(Drawable* drawable)
    {
        m_renderer.AddDrawRequest(drawable);
    }
} // namespace Str