#include "CubeGameTest.h"
#include <StarDust/Model/MeshRegistry.h>

CubeGameTest::CubeGameTest()
    : TestScene(Test::TestType::CubeGame)
{
    static constexpr float CubeSideLength = 1.0f;
    static constexpr float MaxHeight = 200.0f;
    Uni::Alg::Noise::FloatMap2D map{ m_noiseGenerator.Generate(
        m_width, m_height, 100.0f, 7.0f) };

    for (size_t y = 0; y < map.GetHeight(); ++y)
    {
        for (size_t x = 0; x < map.GetWidth(); ++x)
        {
            Uni::Math::Transform transform;
            transform.SetTranslation({
                static_cast<float>(x - (map.GetWidth() / 2UL)) * CubeSideLength,
                static_cast<float>(y - (map.GetHeight() / 2UL)) *
                    CubeSideLength,
                std::floor(map.GetValue(x, y) * MaxHeight) * CubeSideLength,
            });
            transform.SetScale(Uni::Math::Vector3f{ CubeSideLength });

            m_instances.emplace_back(
                Star::Mesh::GetId("Box"),
                transform,
                transform.GetTranslation().m_z < 0.0f
                    ? Uni::Grpx::Color::CreateFromUint8(194, 178, 128)
                    : Uni::Grpx::Color::CreateFromUint8(107, 142, 35));
        }
    }

    Uni::Math::Transform waterTransform;
    waterTransform.SetTranslation({
        0.0f,
        0.0f,
        -2.0f,
    });
    waterTransform.SetRotation(Uni::Math::Quaternion::CreateFromEulerDegZYX(
        Uni::Math::Vector3f{ 0.0f, -90.0f, 0.0f }));
    waterTransform.SetScale(Uni::Math::Vector3f{
        1.0f,
        static_cast<float>(map.GetHeight()) * CubeSideLength,
        static_cast<float>(map.GetWidth()) * CubeSideLength,
    });

    m_instances.emplace_back(
        Star::Mesh::GetId("Rectangle"),
        waterTransform,
        Uni::Grpx::Color::CreateFromUint8(14, 135, 204, 120));
}

CubeGameTest::~CubeGameTest()
{
}

void CubeGameTest::OnUpdate(float deltaTime)
{
    TestScene::OnUpdate(deltaTime);
}

void CubeGameTest::OnRender(Star::Window& window)
{
    TestScene::OnRender(window);
}

void CubeGameTest::OnImGuiRender()
{
    TestScene::OnImGuiRender();
}
