#include <StarDust/Model/ModelInstance.h>
#include <StarDust/Shader/ShaderProgram.h>
#include <StarDust/Shader/ShaderProgramRegistry.h>
#include <StarDust/Window.h>
#include <Universal/Math/Random/Generator.h>
#include <Universal/System/Clock.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>

void Instances()
{
    Str::Window window(1920, 1080, "StarDust");

    const size_t BoxCount = 100000LU;

    std::vector<Str::ModelInstance> boxes;
    std::vector<Uni::Math::Vector3f> velocities;
    boxes.reserve(BoxCount);

    Uni::Math::Rand::Generator generator;
    generator.SetRandomSeed();

    const Uni::Math::Vector3f Max = {10000.0f, 10000.0f, 10000.0f};
    const Uni::Math::Vector3f Min = -Max;

    for (size_t i = 0; i < BoxCount; ++i)
    {
        const Uni::Math::Vector3f dim = Uni::Math::Vector3f::CreateFromFloat(
            generator.GenerateInRange(25.0f, 75.0f));

        Uni::Math::Transform transform;
        transform.SetScale(dim);
        transform.SetTranslation({
            generator.GenerateInRange(Min.m_x, Max.m_x),
            generator.GenerateInRange(Min.m_y, Max.m_y),
            generator.GenerateInRange(Min.m_z, Max.m_z),
        });

        boxes.emplace_back(
            Str::PrimitiveType::Icosahedron,
            transform,
            Uni::Grpx::Color::CreateFromVector3f(
                Uni::Math::Vector3f::CreateRandomUnitVector(generator) * 2.0f,
                1.0f));

        velocities.emplace_back(
            Uni::Math::Vector3f::CreateRandomUnitVector(generator) * 0.01f);
    }


    glm::mat4 view = glm::lookAt(
        glm::vec3(
            0.0f,
            0.0f,
            0.0f), // Camera is at in World Space
        glm::vec3(
            1.0f,
            0.0f,
            0.0f), // and looks at the origin
        glm::vec3(0, 0, 1) // Head is up (set to 0,-1,0 to look upside-down)
    );

    float rotSpeed = 0.0001f;
    Uni::Sys::Clock clock;
    float elapsedTime = 0.1f;
    float speedMultiplier = 0.001f;
    float fieldOfView = 45.0f;
    while (window.IsOpen())
    {
        window.Update();

        ImGui::Text("Instances");
        ImGui::SliderFloat("Rotation Speed", &rotSpeed, 0.00001f, 1.0f);
        ImGui::SliderFloat(
            "Translation Speed", &speedMultiplier, 1.0f, 100.0f);
        ImGui::SliderFloat("FOV", &fieldOfView, 0.0f, 160.0f);

        glm::mat4 proj = glm::perspective(
            glm::radians(fieldOfView),
            static_cast<float>(window.GetWidth()) /
                static_cast<float>(window.GetHeight()),
            0.1f,
            10000.0f);

        for (auto& box : boxes)
        {
            Uni::Math::Matrix3x4f rotation =
                Uni::Math::Matrix3x4f::CreateFromRotationRadians(
                    rotSpeed * 0.001f * elapsedTime, Uni::Math::Axis::X) *
                Uni::Math::Matrix3x4f::CreateFromRotationRadians(
                    rotSpeed * 0.002f * elapsedTime, Uni::Math::Axis::Y) *
                Uni::Math::Matrix3x4f::CreateFromRotationRadians(
                    rotSpeed * 0.003f * elapsedTime, Uni::Math::Axis::Z);

            const Uni::Math::Vector3f currentTranslation =
                box.GetTransform().GetTranslation();

            if (currentTranslation.m_x > Max.m_x ||
                currentTranslation.m_x < Min.m_x)
            {
                velocities[&box - &boxes[0]].m_x *= -1.0f;
            }

            if (currentTranslation.m_y > Max.m_y ||
                currentTranslation.m_y < Min.m_y)
            {
                velocities[&box - &boxes[0]].m_y *= -1.0f;
            }

            if (currentTranslation.m_z > Max.m_z ||
                currentTranslation.m_z < Min.m_z)
            {
                velocities[&box - &boxes[0]].m_z *= -1.0f;
            }

            box.GetTransform().Rotate(rotation);
            box.GetTransform().Translate(
                { velocities[&box -&boxes[0]] * speedMultiplier * elapsedTime });

            window.Draw(box);
        }

        view = glm::rotate(view, rotSpeed * 0.001f * elapsedTime, glm::vec3(1.0f, 2.0f, 3.0f));

        Str::ShaderProgram& shader =
            Str::ShaderProgramRegistry::Get().GetShaderProgram(
                "model_instance");
        shader.Bind();
        shader.SetUniformMat4f("u_view", view);
        shader.SetUniformMat4f("u_proj", proj);

        window.Clear();

        elapsedTime = clock.GetElapsedTime();
        clock.Reset();
    }
}

void Primitive()
{
    Str::Window window(1920, 1080, "StarDust");

    glm::mat4 proj = glm::perspective(
        glm::radians(45.0f),
        static_cast<float>(window.GetWidth()) /
            static_cast<float>(window.GetHeight()),
        0.1f,
        400.0f);
    glm::mat4 view = glm::lookAt(
        glm::vec3(
            50.0f,
            0.0f,
            0.0f), // Camera is at in World Space
        glm::vec3(
            0.0f,
            0.0f,
            0.0f), // and looks at the origin
        glm::vec3(0, 0, 1) // Head is up (set to 0,-1,0 to look upside-down)
    );

    static const std::array<std::pair<std::string, Str::PrimitiveType>, 4>
        Primitives = {
            std::make_pair("Triangle", Str::PrimitiveType::Triangle),
            std::make_pair("Rectangle", Str::PrimitiveType::Rectangle),
            std::make_pair("Box", Str::PrimitiveType::Box),
            std::make_pair("Icosahedron", Str::PrimitiveType::Icosahedron),
        };

    Str::PrimitiveType currentPrimitive = Str::PrimitiveType::Triangle;

    Str::ModelInstance instance(
        currentPrimitive,
        Uni::Math::Transform(),
        Uni::Grpx::Color::CreateFromVector3f(
            Uni::Math::Vector3f::CreateFromFloat(1.0f), 1.0f));

    Uni::Math::Vector3f translation = Uni::Math::Vector3f::CreateZero();
    Uni::Math::Vector3f rotation = Uni::Math::Vector3f::CreateZero();
    float scale = 1.0f;
    std::string currentName = "Triangle";
    while (window.IsOpen())
    {
        window.Update();

        ImGui::Text("Primitive");
        if (ImGui::BeginCombo(
                "PrimitiveType", currentName.c_str(), ImGuiComboFlags_None))
        {
            for (const auto& primitive : Primitives)
            {
                bool isSelected = (currentName == primitive.first);
                if (ImGui::Selectable(
                        primitive.first.c_str(),
                        isSelected,
                        ImGuiSelectableFlags_None))
                {
                    currentName = primitive.first;
                    currentPrimitive = primitive.second;
                    instance = Str::ModelInstance(
                        currentPrimitive,
                        Uni::Math::Transform(),
                        Uni::Grpx::Color::CreateFromVector3f(
                            Uni::Math::Vector3f::CreateFromFloat(1.0f), 1.0f));
                }
            }
            ImGui::EndCombo();
        }
        ImGui::SliderFloat3(
            "Translation (Meters)", &translation.m_x, -10.0f, 10.0f);
        ImGui::SliderFloat3("Rotation (Deg)", &rotation.m_x, -180.0f, 180.0f);
        ImGui::SliderFloat("Scale", &scale, 1.0f, 20.0f);

        instance.GetTransform().SetTranslation(translation);
        instance.GetTransform().SetRotation(
            Uni::Math::Matrix3x4f::CreateFromRotationDegrees(
                rotation.m_x, Uni::Math::Axis::X) *
            Uni::Math::Matrix3x4f::CreateFromRotationDegrees(
                rotation.m_y, Uni::Math::Axis::Y) *
            Uni::Math::Matrix3x4f::CreateFromRotationDegrees(
                rotation.m_z, Uni::Math::Axis::Z));
        instance.GetTransform().SetScale({ scale, scale, scale });

        window.Draw(instance);

        Str::ShaderProgram& shader =
            Str::ShaderProgramRegistry::Get().GetShaderProgram(
                "model_instance");
        shader.Bind();
        shader.SetUniformMat4f("u_view", view);
        shader.SetUniformMat4f("u_proj", proj);

        window.Clear();
    }
}

void Axes()
{
    Str::Window window(1920, 1080, "StarDust");

    glm::mat4 proj = glm::perspective(
        glm::radians(45.0f),
        static_cast<float>(window.GetWidth()) /
            static_cast<float>(window.GetHeight()),
        0.1f,
        1000.0f);
    glm::mat4 view = glm::lookAt(
        glm::vec3(
            80.0f,
            80.0f,
            80.0f), // Camera is at in World Space
        glm::vec3(
            0.0f,
            0.0f,
            0.0f), // and looks at the origin
        glm::vec3(0, 0, 1) // Head is up (set to 0,-1,0 to look upside-down)
    );

    Uni::Math::Transform transformX;
    transformX.SetTranslation({
        10.0f,
        0.0f,
        0.0f,
    });
    transformX.SetScale({
        20.0f,
        1.0f,
        1.0f,
    });

    Uni::Math::Transform transformY;
    transformY.SetTranslation({
        0.0f,
        10.0f,
        0.0f,
    });
    transformY.SetScale({
        1.0f,
        20.0f,
        1.0f,
    });

    Uni::Math::Transform transformZ;
    transformZ.SetTranslation({
        0.0f,
        0.0f,
        10.0f,
    });
    transformZ.SetScale({
        1.0f,
        1.0f,
        20.0f,
    });

    Str::ModelInstance axisX = Str::ModelInstance(
        Str::PrimitiveType::Box, transformX, Uni::Grpx::Color::Red);

    Str::ModelInstance axisY = Str::ModelInstance(
        Str::PrimitiveType::Box, transformY, Uni::Grpx::Color::Green);

    Str::ModelInstance axisZ = Str::ModelInstance(
        Str::PrimitiveType::Box, transformZ, Uni::Grpx::Color::Blue);

    Uni::Math::Vector3f rotation{ 0.0f, 0.0f, 0.0f };
    Uni::Math::Vector3f translation{ 0.0f, 0.0f, 0.0f };
    while (window.IsOpen())
    {
        window.Update();

        ImGui::Text("Coordinate system (X - Red, Y - Green, Z - Blue)");
        ImGui::SliderFloat3(
            "Rotation (Rad):",
            static_cast<float*>(static_cast<void*>(&rotation)),
            0.0f,
            2.0f);
        ImGui::SliderFloat3(
            "Translation (Meters):",
            reinterpret_cast<float*>(static_cast<void*>(&translation)),
            -20.0f,
            20.0f);

        axisX.GetTransform() = transformX;
        axisX.GetTransform().Translate(translation);
        axisX.GetTransform().SetRotation(
            Uni::Math::Matrix3x4f::CreateFromRotationRadians(
                rotation.m_x, Uni::Math::Axis::X) *
            Uni::Math::Matrix3x4f::CreateFromRotationRadians(
                rotation.m_y, Uni::Math::Axis::Y) *
            Uni::Math::Matrix3x4f::CreateFromRotationRadians(
                rotation.m_z, Uni::Math::Axis::Z));

        axisY.GetTransform() = transformY;
        axisY.GetTransform().Translate(translation);
        axisY.GetTransform().SetRotation(
            Uni::Math::Matrix3x4f::CreateFromRotationRadians(
                rotation.m_x, Uni::Math::Axis::X) *
            Uni::Math::Matrix3x4f::CreateFromRotationRadians(
                rotation.m_y, Uni::Math::Axis::Y) *
            Uni::Math::Matrix3x4f::CreateFromRotationRadians(
                rotation.m_z, Uni::Math::Axis::Z));

        axisZ.GetTransform() = transformZ;
        axisZ.GetTransform().Translate(translation);
        axisZ.GetTransform().SetRotation(
            Uni::Math::Matrix3x4f::CreateFromRotationRadians(
                rotation.m_x, Uni::Math::Axis::X) *
            Uni::Math::Matrix3x4f::CreateFromRotationRadians(
                rotation.m_y, Uni::Math::Axis::Y) *
            Uni::Math::Matrix3x4f::CreateFromRotationRadians(
                rotation.m_z, Uni::Math::Axis::Z));

        window.Draw(axisX);
        window.Draw(axisY);
        window.Draw(axisZ);

        Str::ShaderProgram& shader =
            Str::ShaderProgramRegistry::Get().GetShaderProgram(
                "model_instance");
        shader.Bind();
        shader.SetUniformMat4f("u_view", view);
        shader.SetUniformMat4f("u_proj", proj);

        window.Clear();
    }
}

int main()
{
    Instances();
}
