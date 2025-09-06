#include <StarDust/Window.h>
#include <StarDust/Utilities/Utils.h>
#include <glad/glad.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <stdexcept>

namespace Star
{
    namespace
    {
        std::string GetGlslVersion()
        {
            const char* versionStr = reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION));
            if (!versionStr)
            {
                return "#version 130";
            }

            std::istringstream iss(versionStr);
            int major = 0, minor = 0;
            char dot;
            if (iss >> major >> dot >> minor && dot == '.')
            {
                return "#version " + std::to_string(major) + std::to_string(minor);
            }

            return "#version 130";
        }
    }

    Window::Window(int width, int height, const char* title)
        : m_width{ width }
        , m_height{ height }
    {
        if (!glfwInit())
        {
            throw std::runtime_error("Failed to initialize GLFW.");
        }

        glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

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
        GL_CHECK(glEnable(GL_DEPTH_TEST));
        GL_CHECK(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        (void)io;
        io.FontGlobalScale = 2.0f;

        ImGui_ImplGlfw_InitForOpenGL(m_window, true);
        ImGui_ImplOpenGL3_Init(GetGlslVersion().c_str());


        ImGui::StyleColorsDark();

        Renderer::Get().SetPixelizationResolution(
            m_width, m_height, m_width, m_height);
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
        GL_CHECK(glfwSwapBuffers(m_window));
        GL_CHECK(glClear(GL_COLOR_BUFFER_BIT));

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void Window::Update() const
    {
        GL_CHECK(glfwPollEvents());
        Renderer::Get().Render();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void Window::SetPixelizationFactor(unsigned int factor)
    {
        Renderer::Get().SetPixelizationResolution(
            m_width / factor, m_height / factor, m_width, m_height);
    }

    GLFWwindow* Window::GetNativeWindow() const
    {
        return m_window;
    }
} // namespace Star