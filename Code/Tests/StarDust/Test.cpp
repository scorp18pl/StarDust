#include "Test.h"
#include "AxesTest.h"
#include "CubeGameTest.h"
#include "InstancingTest.h"
#include "MeshTest.h"
#include "TerrainGenerationTest.h"

const std::map<Test::TestType, std::string> Test::TestTypeMap{
    { Test::TestType::Axes, "Axes" },
    { Test::TestType::Instancing, "Instancing" },
    { Test::TestType::Mesh, "Mesh" },
    { Test::TestType::Scene, "Scene" },
    { Test::TestType::TerrainGeneration, "Terrain Generation" },
    { Test::TestType::CubeGame, "Cube Game" },
};

Test::Test(Test::TestType type)
{
    m_type = type;
}

Test* Test::CreateTest(Test::TestType type)
{
    switch (type)
    {
    case TestType::Axes:
        return new AxesTest();
    case TestType::Instancing:
        return new InstancingTest();
    case TestType::Mesh:
        return new MeshTest();
    case TestType::Scene:
        return new TestScene();
    case TestType::TerrainGeneration:
        return new TerrainGenerationTest();
    case TestType::CubeGame:
        return new CubeGameTest();
    default:
        return nullptr;
    }
}

const std::string& Test::GetName() const
{
    return TestTypeMap.at(m_type);
}
