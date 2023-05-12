#include <StarDust/Model/ModelInstance.h>
#include <StarDust/Renderer.h>

#include <utility>

namespace Str
{
    ModelInstance::ModelInstance(
        PrimitiveType primitiveType,
        const Uni::Math::Transform& transform,
        const Uni::Grpx::Color& color)
        : m_instanceId{ Renderer::Get().RegisterModelInstance(primitiveType) }
        , m_primitiveType{ primitiveType }
        , m_transform{ transform }
        , m_color{ color }
    {
        InstanceData& instanceData =
            Renderer::Get().GetInstanceData(primitiveType, m_instanceId);

        instanceData.m_transform = m_transform.GetMatrix();
        instanceData.m_normalTransform = m_transform.GetRotation();
        instanceData.m_color = m_color;
    }

    ModelInstance::ModelInstance(const ModelInstance& other)
    {
        (*this) = other;
    }

    ModelInstance::ModelInstance(ModelInstance&& other) noexcept
        : m_instanceId{ other.m_instanceId }
        , m_modelName{ std::move(other.m_modelName) }
        , m_primitiveType{ std::exchange(other.m_primitiveType, PrimitiveType::None) }
        , m_transform{ other.m_transform }
        , m_color{ other.m_color }
    {
    }

    ModelInstance::~ModelInstance()
    {
        Renderer::Get().UnregisterModelInstance(m_primitiveType, m_instanceId);
    }

    ModelInstance& ModelInstance::operator=(const ModelInstance& other)
    {
        m_primitiveType = other.m_primitiveType;
        m_transform = other.m_transform;
        m_color = other.m_color;
        m_instanceId = Renderer::Get().RegisterModelInstance(m_primitiveType);

        return *this;
    }

    const std::string& ModelInstance::GetModelName() const
    {
        return m_modelName;
    }

    PrimitiveType ModelInstance::GetPrimitiveType() const
    {
        return m_primitiveType;
    }

    const Uni::Math::Transform& ModelInstance::GetTransform() const
    {
        return m_transform;
    }

    const Uni::Grpx::Color& ModelInstance::GetColor() const
    {
        return m_color;
    }

    Uni::Math::Transform& ModelInstance::GetTransform()
    {
        return m_transform;
    }

    Uni::Grpx::Color& ModelInstance::GetColor()
    {
        return m_color;
    }

    void ModelInstance::SetPrimitiveType(PrimitiveType primitiveType)
    {
        m_primitiveType = primitiveType;
    }

    void ModelInstance::Update()
    {
        InstanceData& instanceData =
            Renderer::Get().GetInstanceData(m_primitiveType, m_instanceId);

        instanceData.m_transform = m_transform.GetMatrix();
        instanceData.m_normalTransform = m_transform.GetRotation();
        instanceData.m_color = m_color;
    }
} // namespace Str