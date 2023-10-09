#pragma once

#include <StarDust/Window.h>
#include <map>
#include <string>

class Test
{
public:
    enum class TestType
    {
        Axes,
        Instancing,
        Mesh,
        Scene,
        TerrainGeneration,
        CubeGame,
        GameOfLife,
        Sprite,
    };

    static const std::map<TestType, std::string> TestTypeMap;

    static Test* CreateTest(TestType type);

    explicit Test(TestType type);
    Test(const Test&) = default;
    Test(Test&&) = delete;
    virtual ~Test() = default;

    [[nodiscard]] const std::string& GetName() const;

    virtual void OnUpdate(float deltaTime){};
    virtual void OnRender(Star::Window& window){};
    virtual void OnImGuiRender(){};

    Test& operator=(const Test&) = default;

private:
    TestType m_type;
};
