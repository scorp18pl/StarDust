#include "Test.h"
#include "AxesTest.h"
#include "InstancingTest.h"
#include "PrimitiveTest.h"
#include "TestScene.h"

const std::map<Test::TestType, std::string> Test::TestTypeMap{
    { Test::TestType::Axes, "Axes" },
    { Test::TestType::Instancing, "Instancing" },
    { Test::TestType::Primitive, "Primitive" },
    { Test::TestType::Scene, "Scene" },
    { Test::TestType::Stress, "Stress" },
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
    case TestType::Primitive:
        return new PrimitiveTest();
    case TestType::Scene:
        return new TestScene();
    default:
        return nullptr;
    }
}

const std::string& Test::GetName() const
{
    return TestTypeMap.at(m_type);
}
