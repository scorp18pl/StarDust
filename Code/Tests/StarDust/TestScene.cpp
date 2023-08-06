#include "TestScene.h"
#include <GLFW/glfw3.h>
#include <StarDust/Model/MeshRegistry.h>
#include <StarDust/Shader/ShaderProgramRegistry.h>
#include <StarDust/Utilities/Math.h>
#include <Universal/Math/Math.h>
#include <imgui/imgui.h>

TestScene::TestScene(TestType testType)
    : Test(testType)
{
    m_cameraYaw = 0.0f;
    m_cameraPitch = 20.0f;

    m_cameraTranslation = Uni::Math::Vector3f{
        -20.0f,
        0.0f,
        20.0f,
    };
}

void TestScene::OnUpdate(float deltaTime)
{
    m_deltaTime = deltaTime;
}

void TestScene::OnRender(Star::Window& window)
{
    HandleCamera(window);

    if (m_Perspective)
    {
        m_aspectRatio = static_cast<float>(window.GetWidth()) /
            static_cast<float>(window.GetHeight());

        m_projectionMatrix = Star::Utils::CreatePerspectiveProjectionMatrix(
            m_fieldOfView * Uni::Math::Constants::DegToRad,
            m_aspectRatio,
            0.1f,
            1000.0f);
    }
    else
    {
        m_projectionMatrix = Star::Utils::CreateOrthographicProjectionMatrix(
            -m_orthoSize.m_x,
            m_orthoSize.m_x,
            -m_orthoSize.m_y,
            m_orthoSize.m_y,
            -m_orthoSize.m_z,
            m_orthoSize.m_z);
    }

    Star::ShaderProgram& shaderProgram =
        Star::ShaderProgramRegistry::Get().GetShaderProgram("model_instance");

    shaderProgram.SetUniformMat4x4f("u_proj", m_projectionMatrix);

    shaderProgram.SetUniformMat4x4f("u_view", m_viewMatrix);

    shaderProgram.SetUniform3f(
        "u_viewPosition",
        m_cameraTranslation.m_x,
        m_cameraTranslation.m_y,
        m_cameraTranslation.m_z);
}

void TestScene::OnImGuiRender()
{
    if (ImGui::CollapsingHeader("Camera"))
    {
        ImGui::Indent(4.0f);
        ImGui::Checkbox("Perspective", &m_Perspective);
        if (m_Perspective)
        {
            ImGui::SliderFloat("Field of View", &m_fieldOfView, 0.0f, 180.0f);
        }
        else
        {
            ImGui::InputFloat3("Orthographic Size", &m_orthoSize.m_x);
        }

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
                    Star::MeshRegistry::Get().GetMeshId("Icosahedron"),
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
            Uni::Math::Vector3f rotation = m_instances[m_currentInstance]
                                               .GetTransform()
                                               .GetRotation()
                                               .GetEulerDegZYX();

            Uni::Grpx::Color& color = m_instances[m_currentInstance].GetColor();

            const auto& meshes =
                Star::MeshRegistry::Get().GetRegisteredMeshes();
            Star::Mesh::IdType selectedMeshId =
                m_instances[m_currentInstance].GetMeshId();

            if (ImGui::BeginCombo(
                    "PrimitiveType",
                    meshes.at(selectedMeshId).m_name.c_str(),
                    ImGuiComboFlags_None))
            {
                for (const auto& [meshId, mesh] : meshes)
                {
                    bool isSelected = (meshId == selectedMeshId);
                    if (ImGui::Selectable(
                            mesh.m_name.c_str(),
                            isSelected,
                            ImGuiSelectableFlags_None))
                    {
                        m_instances[m_currentInstance].SetMeshId(meshId);
                    }
                }
                ImGui::EndCombo();
            }

            ImGui::InputFloat3("Translation (Meters)", translation.m_data);
            if (ImGui::Button("Reset Translation"))
            {
                translation = Uni::Math::Vector3f{ 0.0f };
            }

            ImGui::InputFloat3("Scale", scale.m_data);
            if (ImGui::Button("Reset Scale"))
            {
                scale = Uni::Math::Vector3f{ 1.0f };
            }

            ImGui::InputFloat3("Rotation (X, Y, Z; Radians)", rotation.m_data);
            if (ImGui::Button("Reset Rotation"))
            {
                rotation = Uni::Math::Vector3f{ 0.0f };
            }

            m_instances[m_currentInstance].GetTransform().SetTranslation(
                translation);
            m_instances[m_currentInstance].GetTransform().SetScale(scale);
            m_instances[m_currentInstance].GetTransform().SetRotation(
                Uni::Math::Quaternion::CreateFromEulerDegZYX(rotation));

            ImGui::ColorEdit3("Color", color.m_data);

            m_instances[m_currentInstance].Update();

            ImGui::Unindent(4.0f);
        }
    }

    if (ImGui::CollapsingHeader("Point Lights"))
    {
        ImGui::Indent(4.0f);
        ImGui::Text("Point Light count: %d", m_lights.size());

        if (ImGui::BeginCombo(
                "Select point light",
                std::string(
                    std::string("Light Source: ") +
                    std::to_string(m_currentLight))
                    .c_str(),
                ImGuiComboFlags_None))
        {
            for (const auto& pointLight : m_lights)
            {
                bool isSelected =
                    (m_currentLight == &pointLight - &m_lights[0]);
                if (ImGui::Selectable(
                        std::string(
                            std::string("Light Source: ") +
                            std::to_string(&pointLight - &m_lights[0]))
                            .c_str(),
                        isSelected,
                        ImGuiSelectableFlags_None))
                {
                    m_currentLight = &pointLight - &m_lights[0];
                }
            }

            if (ImGui::Selectable("Add Point Light"))
            {
                m_currentLight = m_lights.size();
                m_lights.emplace_back(
                    Star::Renderer::Get().RegisterLightSource(
                        Star::LightSourceType::Point),
                    Star::LightSourceType::Point);
            }

            if (ImGui::Selectable("Add Directional Light"))
            {
                m_currentLight = m_lights.size();
                m_lights.emplace_back(
                    Star::Renderer::Get().RegisterLightSource(
                        Star::LightSourceType::Directional),
                    Star::LightSourceType::Directional);
            }

            ImGui::EndCombo();
        }

        if (m_currentLight < m_lights.size())
        {
            ImGui::Text("Selected Light Source: %d", m_currentLight);
            ImGui::Indent(4.0f);

            Star::LightData& pointLight =
                Star::Renderer::Get().GetLightSourceData(
                    m_lights[m_currentLight].second,
                    m_lights[m_currentLight].first);

            ImGui::InputFloat3(
                (m_lights[m_currentLight].second == Star::LightSourceType::Point
                     ? "Position"
                     : "Direction"),
                reinterpret_cast<float*>(&pointLight.m_vector));
            ImGui::ColorEdit3(
                "Light Color", reinterpret_cast<float*>(&pointLight.m_color));
            ImGui::InputFloat("Intensity", &pointLight.m_intensity);

            ImGui::Unindent(4.0f);
        }

        ImGui::Indent(4.0f);
    }
}

void TestScene::HandleCamera(Star::Window& window)
{
    static float sensitivity = 1.0e-2f;

    GLFWwindow* glfwWindow = window.GetNativeWindow();
    auto windowWidth = static_cast<float>(window.GetWidth());
    auto windowHeight = static_cast<float>(window.GetHeight());

    if (glfwGetKey(glfwWindow, GLFW_KEY_W) == GLFW_PRESS)
    {
        m_cameraTranslation += sensitivity * m_deltaTime *
            (Uni::Math::Matrix3x4f::CreateRotationEulerDegrees(
                 m_cameraYaw, Uni::Math::Axis::Z) *
             Uni::Math::Vector3f{ 1.0f, 0.0f, 0.0f });
    }
    if (glfwGetKey(glfwWindow, GLFW_KEY_A) == GLFW_PRESS)
    {
        m_cameraTranslation += sensitivity * m_deltaTime *
            (Uni::Math::Matrix3x4f::CreateRotationEulerDegrees(
                 m_cameraYaw, Uni::Math::Axis::Z) *
             Uni::Math::Vector3f{ 0.0f, 1.0f, 0.0f });
    }
    if (glfwGetKey(glfwWindow, GLFW_KEY_S) == GLFW_PRESS)
    {
        m_cameraTranslation += sensitivity * m_deltaTime *
            (Uni::Math::Matrix3x4f::CreateRotationEulerDegrees(
                 m_cameraYaw, Uni::Math::Axis::Z) *
             Uni::Math::Vector3f{ -1.0f, 0.0f, 0.0f });
    }
    if (glfwGetKey(glfwWindow, GLFW_KEY_D) == GLFW_PRESS)
    {
        m_cameraTranslation += sensitivity * m_deltaTime *
            (Uni::Math::Matrix3x4f::CreateRotationEulerDegrees(
                 m_cameraYaw, Uni::Math::Axis::Z) *
             Uni::Math::Vector3f{ 0.0f, -1.0f, 0.0f });
    }

    if (glfwGetKey(glfwWindow, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        m_cameraTranslation += sensitivity * m_deltaTime *
            Uni::Math::Vector3f{ 0.0f, 0.0f, -1.0f };
    }

    if (glfwGetKey(glfwWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        m_cameraTranslation +=
            sensitivity * m_deltaTime * Uni::Math::Vector3f{ 0.0f, 0.0f, 1.0f };
    }

    static bool last = false;
    if (glfwGetKey(glfwWindow, GLFW_KEY_Q) == GLFW_PRESS && last)
    {
        m_cameraMode = !m_cameraMode;
    }

    last = glfwGetKey(glfwWindow, GLFW_KEY_Q) == GLFW_RELEASE;

    if (m_cameraMode)
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
        {
            m_cameraPitch = 89.0f;
        }

        if (m_cameraPitch < -89.0f)
        {
            m_cameraPitch = -89.0f;
        }
    }
    else
    {
        glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    const Uni::Math::Matrix3x4f cameraRotation =
        Uni::Math::Matrix3x4f::CreateRotationEulerDegrees(
            m_cameraYaw, Uni::Math::Axis::Z) *
        Uni::Math::Matrix3x4f::CreateRotationEulerDegrees(
            m_cameraPitch, Uni::Math::Axis::Y);

    Uni::Math::Vector3f cameraFront = m_cameraTranslation +
        cameraRotation * Uni::Math::Vector3f::CreateAxisX();

    Uni::Math::Vector3f cameraUp =  cameraRotation * Uni::Math::Vector3f::CreateAxisZ();

    m_viewMatrix = Star::Utils::CreateLookAtMatrix(
        m_cameraTranslation, cameraFront, cameraUp);
}
