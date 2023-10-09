#include "TerrainGenerationTest.h"
#include <StarDust/Model/MeshRegistry.h>
#include <StarDust/Model/ModelInstanceSystem.h>

TerrainGenerationTest::TerrainGenerationTest()
    : TestScene(TestType::TerrainGeneration)
    , m_model({})
    , m_lightSource(
          Star::LightSourceType::Directional,
          Star::LightData{
              { 0.0f, -0.707f, -0.707f }, Uni::Grpx::Color::White, 1.0f })
{
    static constexpr float MaxHeight = 100.0f;
    Uni::Alg::Noise::FloatMap2D map{ m_noiseGenerator.Generate(
        m_width, m_height, 100.0f, 7.0f) };

    Star::Mesh mesh;
    mesh.m_name = "Terrain";
    for (size_t y = 0; y < map.GetHeight() - 1; ++y)
    {
        for (size_t x = 0; x < map.GetWidth() - 1; ++x)
        {
            Uni::Math::Vector3f northWest, northEast, southWest, southEast;
            northWest = {
                static_cast<float>(x),
                static_cast<float>(y),
                map.GetValue(x, y) * MaxHeight,
            };

            northEast = {
                static_cast<float>(x + 1),
                static_cast<float>(y),
                map.GetValue(x + 1, y) * MaxHeight,
            };

            southWest = {
                static_cast<float>(x),
                static_cast<float>(y + 1),
                map.GetValue(x, y + 1) * MaxHeight,
            };

            southEast = {
                static_cast<float>(x + 1),
                static_cast<float>(y + 1),
                map.GetValue(x + 1, y + 1) * MaxHeight,
            };

            Uni::Math::Vector3f normal1, normal2;
            normal1 =
                (northEast - northWest).GetCrossProduct(southWest - northWest);
            normal2 =
                (southWest - southEast).GetCrossProduct(northEast - southEast);

            mesh.m_vertices.push_back(Star::Vertex{ northWest, normal1, {} });
            mesh.m_vertices.push_back(Star::Vertex{ northEast, normal1, {} });
            mesh.m_vertices.push_back(Star::Vertex{ southWest, normal1, {} });

            mesh.m_indices.push_back(mesh.m_vertices.size() - 3);
            mesh.m_indices.push_back(mesh.m_vertices.size() - 2);
            mesh.m_indices.push_back(mesh.m_vertices.size() - 1);

            mesh.m_vertices.push_back(Star::Vertex{ southWest, normal2, {} });
            mesh.m_vertices.push_back(Star::Vertex{ northEast, normal2, {} });
            mesh.m_vertices.push_back(Star::Vertex{ southEast, normal2, {} });

            mesh.m_indices.push_back(mesh.m_vertices.size() - 3);
            mesh.m_indices.push_back(mesh.m_vertices.size() - 2);
            mesh.m_indices.push_back(mesh.m_vertices.size() - 1);
        }
    }
    Star::MeshRegistry::Get().RegisterMesh(mesh);
    m_instances.emplace_back(mesh.GetId());
}

void TerrainGenerationTest::OnUpdate(float deltaTime)
{
    TestScene::OnUpdate(deltaTime);
}

void TerrainGenerationTest::OnRender(Star::Window& window)
{
    TestScene::OnRender(window);
}

void TerrainGenerationTest::OnImGuiRender()
{
    TestScene::OnImGuiRender();
}

TerrainGenerationTest::~TerrainGenerationTest()
{
    Star::MeshRegistry::Get().UnregisterMesh("Terrain");
}
