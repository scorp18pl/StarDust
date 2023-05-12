#include "Test.h"
#include <glm/mat4x4.hpp>
#include <vector>
#include <StarDust/LightSource.h>

class TestScene : public Test
{
public:
    TestScene();
    ~TestScene() = default;

    void OnUpdate(float deltaTime) override;
    void OnRender(Str::Window& window) override;
    void OnImGuiRender() override;

private:
    std::vector<Str::ModelInstance> m_instances;
    unsigned int m_currentInstance = 0U;

    std::vector<std::pair<int, Str::LightSourceType>> m_lights;
    unsigned int m_currentLight = 0U;

    glm::mat4 m_viewMatrix;

    Uni::Math::Vector3f m_cameraTranslation;
    float m_cameraPitch = 0.0f;
    float m_cameraYaw = 0.0f;

    bool m_cameraMode = false;

    bool m_Perspective = true;
    float m_fieldOfView = 45.0f;
    float m_aspectRatio = 1.0f;
    glm::mat4 m_projectionMatrix;

    void HandleCamera(Str::Window& window);
};
