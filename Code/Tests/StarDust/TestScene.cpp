#include "TestScene.h"
#include <GLFW/glfw3.h>
#include <StarDust/Model/ModelRegistry.h>
#include <StarDust/Shader/ShaderProgramRegistry.h>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui/imgui.h>

TestScene::TestScene()
    : Test(Test::TestType::Scene)
{
    m_cameraYaw = 0.0f;
    m_cameraPitch = 20.0f;

    m_cameraTranslation = Uni::Math::Vector3f{
        -20.0f,
        0.0f,
        20.0f,
    };

    m_instances.emplace_back(
        Str::PrimitiveType::Box,
        Uni::Math::Transform{
            Uni::Math::Matrix3x4f::CreateIdentity(),
            Uni::Math::Vector3f{ 100.0f, 100.0f, 1.0f },
            Uni::Math::Vector3f::CreateZero(),
        },
        Uni::Grpx::Color::White);

    m_instances.emplace_back(
        Str::PrimitiveType::Icosahedron,
        Uni::Math::Transform{
            Uni::Math::Matrix3x4f::CreateIdentity(),
            Uni::Math::Vector3f::CreateFromFloat(4.0f),
            Uni::Math::Vector3f{ 0.0f, 0.0f, 4.0f },
        },
        Uni::Grpx::Color::Red);
}

void TestScene::OnUpdate(float deltaTime)
{
    m_instances[1].GetTransform().Rotate(
        Uni::Math::Matrix3x4f::CreateFromRotationRadians(
            0.001f * deltaTime, Uni::Math::Axis::X) *
        Uni::Math::Matrix3x4f::CreateFromRotationRadians(
            0.001f * deltaTime, Uni::Math::Axis::Y) *
        Uni::Math::Matrix3x4f::CreateFromRotationRadians(
            0.001f * deltaTime, Uni::Math::Axis::Z));
}

void TestScene::OnRender(Str::Window& window)
{
    HandleCamera(window);

    if (m_Perspective)
    {
        m_aspectRatio = static_cast<float>(window.GetWidth()) /
            static_cast<float>(window.GetHeight());

        m_projectionMatrix = glm::perspective(
            glm::radians(m_fieldOfView), m_aspectRatio, 0.1f, 1000.0f);
    }
    else
    {
        m_projectionMatrix =
            glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, -100.f, 100.0f);
    }

    Str::ShaderProgram& shaderProgram =
        Str::ShaderProgramRegistry::Get().GetShaderProgram("model_instance");

    shaderProgram.SetUniformMat4f("u_proj", m_projectionMatrix);

    shaderProgram.SetUniformMat4f("u_view", m_viewMatrix);

    shaderProgram.SetUniform3f(
        "u_viewPosition",
        m_cameraTranslation.m_x,
        m_cameraTranslation.m_y,
        m_cameraTranslation.m_z);

    for (auto& instance : m_instances)
    {
        window.Draw(instance);
    }
}

void TestScene::OnImGuiRender()
{
    static const std::array<std::pair<std::string, Str::PrimitiveType>, 4>
        Primitives = {
            std::make_pair("Triangle", Str::PrimitiveType::Triangle),
            std::make_pair("Rectangle", Str::PrimitiveType::Rectangle),
            std::make_pair("Box", Str::PrimitiveType::Box),
            std::make_pair("Icosahedron", Str::PrimitiveType::Icosahedron),
        };

    if (ImGui::CollapsingHeader("Camera"))
    {
        ImGui::Indent(4.0f);
        ImGui::Checkbox("Perspective", &m_Perspective);
        ImGui::SliderFloat("Field of View", &m_fieldOfView, 0.0f, 180.0f);

        ImGui::Text(
            "Camera Position: (%f, %f, %f)",
            m_cameraTranslation.m_x,
            m_cameraTranslation.m_y,
            m_cameraTranslation.m_z);

        ImGui::InputFloat3("Camera Position", &m_cameraTranslation.m_x);

        ImGui::Text(
            "Camera Rotation (Pitch, Yaw): (%f, %f)",
            m_cameraPitch,
            m_cameraYaw);

        ImGui::InputFloat("Pitch", &m_cameraPitch);
        ImGui::SameLine();
        ImGui::InputFloat("Yaw", &m_cameraYaw);

        ImGui::Indent(4.0f);
    }

    if (ImGui::CollapsingHeader("Primitives"))
    {
        ImGui::Indent(4.0f);
        ImGui::Text("Primitive instance count: %d", m_instances.size());

        if (ImGui::BeginCombo(
                "Select instance",
                std::string(
                    std::string("instance: ") +
                    std::to_string(m_currentInstance))
                    .c_str(),
                ImGuiComboFlags_None))
        {
            for (const auto& instance : m_instances)
            {
                bool isSelected =
                    (m_currentInstance == &instance - &m_instances[0]);
                if (ImGui::Selectable(
                        std::string(
                            std::string("instance: ") +
                            std::to_string(&instance - &m_instances[0]))
                            .c_str(),
                        isSelected,
                        ImGuiSelectableFlags_None))
                {
                    m_currentInstance = &instance - &m_instances[0];
                }
            }

            if (ImGui::Selectable("Add Instance"))
            {
                m_currentInstance = m_instances.size();
                m_instances.emplace_back(
                    Str::PrimitiveType::Icosahedron,
                    Uni::Math::Transform(),
                    Uni::Grpx::Color::White);
            }

            ImGui::EndCombo();
        }

        if (m_currentInstance < m_instances.size())
        {
            ImGui::Text("Selected instance: %d", m_currentInstance);
            ImGui::Indent(4.0f);

            Uni::Math::Vector3f translation =
                m_instances[m_currentInstance].GetTransform().GetTranslation();
            Uni::Math::Vector3f scale =
                m_instances[m_currentInstance].GetTransform().GetScale();

            Uni::Grpx::Color& color = m_instances[m_currentInstance].GetColor();

            if (ImGui::BeginCombo(
                    "PrimitiveType",
                    Str::ModelRegistry::GetPrimitiveModelName(
                        m_instances[m_currentInstance].GetPrimitiveType())
                        .c_str(),
                    ImGuiComboFlags_None))
            {
                for (const auto& primitive : Primitives)
                {
                    bool isSelected =
                        (Str::ModelRegistry::GetPrimitiveModelName(
                             m_instances[m_currentInstance]
                                 .GetPrimitiveType()) == primitive.first);
                    if (ImGui::Selectable(
                            primitive.first.c_str(),
                            isSelected,
                            ImGuiSelectableFlags_None))
                    {
                        m_instances[m_currentInstance].SetPrimitiveType(
                            primitive.second);
                    }
                }
                ImGui::EndCombo();
            }

            ImGui::InputFloat3(
                "Translation (Meters)", reinterpret_cast<float*>(&translation));
            if (ImGui::Button("Reset Translation"))
            {
                translation = Uni::Math::Vector3f::CreateFromFloat(0.0f);
            }

            ImGui::InputFloat3("Scale", reinterpret_cast<float*>(&scale));
            if (ImGui::Button("Reset Scale"))
            {
                scale = Uni::Math::Vector3f::CreateFromFloat(1.0f);
            }

            m_instances[m_currentInstance].GetTransform().SetTranslation(
                translation);
            m_instances[m_currentInstance].GetTransform().SetScale(scale);

            ImGui::ColorEdit3(
                "Color", static_cast<float*>(static_cast<void*>(&color)));

            ImGui::Unindent(4.0f);
        }

        ImGui::Indent(4.0f);
    }
}

void TestScene::HandleCamera(Str::Window& window)
{
    static float sensitivity = 0.01f;

    GLFWwindow* glfwWindow = window.GetNativeWindow();
    auto windowWidth = static_cast<float>(window.GetWidth());
    auto windowHeight = static_cast<float>(window.GetHeight());

    if (glfwGetKey(glfwWindow, GLFW_KEY_W) == GLFW_PRESS)
    {
        m_cameraTranslation += sensitivity *
            (Uni::Math::Matrix3x4f::CreateFromRotationDegrees(
                 m_cameraYaw, Uni::Math::Axis::Z) *
             Uni::Math::Vector3f{ 1.0f, 0.0f, 0.0f });
    }
    if (glfwGetKey(glfwWindow, GLFW_KEY_A) == GLFW_PRESS)
    {
        m_cameraTranslation += sensitivity *
            (Uni::Math::Matrix3x4f::CreateFromRotationDegrees(
                 m_cameraYaw, Uni::Math::Axis::Z) *
             Uni::Math::Vector3f{ 0.0f, 1.0f, 0.0f });
    }
    if (glfwGetKey(glfwWindow, GLFW_KEY_S) == GLFW_PRESS)
    {
        m_cameraTranslation += sensitivity *
            (Uni::Math::Matrix3x4f::CreateFromRotationDegrees(
                 m_cameraYaw, Uni::Math::Axis::Z) *
             Uni::Math::Vector3f{ -1.0f, 0.0f, 0.0f });
    }
    if (glfwGetKey(glfwWindow, GLFW_KEY_D) == GLFW_PRESS)
    {
        m_cameraTranslation += sensitivity *
            (Uni::Math::Matrix3x4f::CreateFromRotationDegrees(
                 m_cameraYaw, Uni::Math::Axis::Z) *
             Uni::Math::Vector3f{ 0.0f, -1.0f, 0.0f });
    }

    if (glfwGetKey(glfwWindow, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        m_cameraTranslation +=
            sensitivity * Uni::Math::Vector3f{ 0.0f, 0.0f, -1.0f };
    }

    if (glfwGetKey(glfwWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        m_cameraTranslation +=
            sensitivity * Uni::Math::Vector3f{ 0.0f, 0.0f, 1.0f };
    }

    if (glfwGetKey(glfwWindow, GLFW_KEY_Q) != GLFW_PRESS)
    {
        glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        double xpos, ypos;
        // get cursor position
        glfwGetCursorPos(glfwWindow, &xpos, &ypos);
        glfwSetCursorPos(glfwWindow, windowWidth / 2.0f, windowHeight / 2.0f);

        static double lastX = xpos;
        static double lastY = ypos;

        double xoffset = xpos - windowWidth / 2.0;
        double yoffset = windowHeight / 2.0 - ypos;

        lastX = xpos;
        lastY = ypos;

        xoffset *= 10.0f * sensitivity;
        yoffset *= 10.0f * sensitivity;

        m_cameraYaw -= static_cast<float>(xoffset);
        m_cameraPitch -= static_cast<float>(yoffset);

        if (m_cameraPitch > 89.0f)
            m_cameraPitch = 89.0f;
        if (m_cameraPitch < -89.0f)
            m_cameraPitch = -89.0f;
    }
    else
    {
        glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    Uni::Math::Vector3f cameraFront = m_cameraTranslation +
        Uni::Math::Matrix3x4f::CreateFromRotationDegrees(
            m_cameraYaw, Uni::Math::Axis::Z) *
            Uni::Math::Matrix3x4f::CreateFromRotationDegrees(
                m_cameraPitch, Uni::Math::Axis::Y) *
            Uni::Math::Vector3f{ 1.0f, 0.0f, 0.0f };

    Uni::Math::Vector3f cameraUp = Uni::Math::Vector3f{ 0.0f, 0.0f, 1.0f };

    m_viewMatrix = glm::lookAt(
        glm::vec3(
            m_cameraTranslation.m_x,
            m_cameraTranslation.m_y,
            m_cameraTranslation.m_z), // Camera is at in World Space
        glm::vec3(
            cameraFront.m_x,
            cameraFront.m_y,
            cameraFront.m_z), // and looks at the origin
        glm::vec3(cameraUp.m_x, cameraUp.m_y, cameraUp.m_z) // Head is up
    );
}
