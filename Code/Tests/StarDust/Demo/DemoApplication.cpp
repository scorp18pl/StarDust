#include <StarDust/Drawable/Box.h>
#include <StarDust/Shader.h>
#include <StarDust/Window.h>
#include <Universal/Math/Random/Generator.h>
#include <Universal/System/Clock.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>

int main()
{
    Str::Window window(1920, 1080, "StarDust");

    Str::Shader shader(
        "/home/scorp/Documents/code/repos/StarDust/Resources/Shaders/"
        "default.vert",
        "/home/scorp/Documents/code/repos/StarDust/Resources/Shaders/"
        "default.frag");

    const size_t BoxCount = 100LU;

    std::vector<Str::Box> boxes;
    boxes.reserve(BoxCount);

    Uni::Math::Rand::Generator generator;
    generator.SetRandomSeed();

    for (size_t i = 0; i < BoxCount; ++i)
    {
        const Uni::Math::Vector3f dim =
            Uni::Math::Vector3f::CreateFromFloat(60.0f);

        Uni::Math::Transform transform;
        transform.SetScale(dim);
        transform.SetTranslation({
            generator.GenerateInRange(-0.25f, 0.25f) *
                static_cast<float>(window.GetWidth()),
            generator.GenerateInRange(-0.25f, 0.25f) *
                static_cast<float>(window.GetHeight()),
            0.0f,
        });

        boxes.emplace_back(
            transform,
            Uni::Grpx::Color::CreateFromVector3f(
                Uni::Math::Vector3f::CreateRandomUnitVector(generator) +
                Uni::Math::Vector3f::CreateFromFloat(0.5f)));
    }

    glm::mat4 proj = glm::perspective(
        glm::radians(45.0f),
        static_cast<float>(window.GetWidth()) /
            static_cast<float>(window.GetHeight()),
        0.1f,
        1000.0f);
    glm::mat4 view = glm::lookAt(
        glm::vec3(
            0.0f,
            0.0f,
            -800.0f), // Camera is at in World Space
        glm::vec3(
            0.0f,
            0.0f,
            0), // and looks at the origin
        glm::vec3(0, 1, 0) // Head is up (set to 0,-1,0 to look upside-down)
    );
    glm::mat4 vp = proj * view;

    float rotSpeed = 0.0001f;
    float translationSpeed = 0.0001f;
    Uni::Sys::Clock clock;
    Uni::Sys::Clock clock2;
    float elapsedTime = 0.1f;
    float dir = 1.0f;
    float boundary = 200.0f;
    while (window.IsOpen())
    {
        window.Update();

        ImGui::Text("Hello, world!");
        ImGui::SliderFloat("Rotation Speed", &rotSpeed, 0.00001f, 1.0f);
        ImGui::SliderFloat(
            "Translation Speed", &translationSpeed, 0.00001f, 1.0f);
        ImGui::SliderFloat("Boundary", &boundary, 0.0f, 500.0f);

        for (auto& box : boxes)
        {
            Uni::Math::Matrix3x4f rotation =
                Uni::Math::Matrix3x4f::CreateFromRotationRadians(
                    rotSpeed * 0.0001f * elapsedTime, Uni::Math::Axis::X) *
                Uni::Math::Matrix3x4f::CreateFromRotationRadians(
                    rotSpeed * 0.0001f * elapsedTime, Uni::Math::Axis::Y) *
                Uni::Math::Matrix3x4f::CreateFromRotationRadians(
                    rotSpeed * 0.0001f * elapsedTime, Uni::Math::Axis::Z);

            const Uni::Math::Vector3f currentTranslation =
                box.GetTransform().GetTranslation();
            if (currentTranslation.m_z > boundary)
            {
                dir = -1.0f;
            }
            else if (currentTranslation.m_z < -boundary)
            {
                dir = 1.0f;
            }

            box.GetTransform().Rotate(rotation);
            box.GetTransform().Translate(
                { 0.0f, 0.0f, dir * translationSpeed * elapsedTime });

            window.Draw(&box);
        }

        shader.Bind();
        shader.SetUniformMat4f("u_VP", vp);

        window.Clear();
        elapsedTime = clock.GetElapsedTime();
        clock.Reset();
    }
}
