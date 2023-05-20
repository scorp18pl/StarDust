#include <StarDust/Model/ModelInstance.h>
#include <StarDust/Model/ModelInstanceSystem.h>

namespace Star
{
    ModelInstance::ModelInstance(
        Mesh::IdType meshId,
        const Uni::Math::Transform& transform,
        const Uni::Grpx::Color& color)
        : m_instanceId{ ModelInstanceSystem::Get().RegisterModelInstance(
              meshId) }
        , m_meshId{ meshId }
        , m_transform{ transform }
        , m_color{ color }
    {
        ModelInstanceData& instanceData =
            ModelInstanceSystem::Get().GetInstanceData(m_instanceId);

        instanceData.m_transform = m_transform.GetMatrix();
        instanceData.m_normalTransform = m_transform.GetRotation().GetMatrix();
        instanceData.m_color = m_color;
    }

    ModelInstance::ModelInstance(const ModelInstance& other)
    {
        (*this) = other;
    }

    ModelInstance::ModelInstance(ModelInstance&& other) noexcept
        : m_instanceId{ other.m_instanceId }
        , m_meshId{ other.m_meshId }
        , m_transform{ other.m_transform }
        , m_color{ other.m_color }
    {
        other.m_instanceId = ModelInstance::InvalidId;
    }

    ModelInstance::~ModelInstance()
    {
        if (m_instanceId != ModelInstance::InvalidId)
        {
            ModelInstanceSystem::Get().UnregisterModelInstance(m_instanceId);
        }
    }

    Mesh::IdType ModelInstance::GetMeshId() const
    {
        return m_meshId;
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

    void ModelInstance::SetMeshId(Mesh::IdType meshId)
    {
        m_meshId = meshId;
        ModelInstanceSystem::Get().UpdateModelInstanceMeshId(
            m_instanceId, m_meshId);
    }

    void ModelInstance::Update()
    {
        ModelInstanceData& instanceData =
            ModelInstanceSystem::Get().GetInstanceData(m_instanceId);

        instanceData.m_transform = m_transform.GetMatrix();
        instanceData.m_normalTransform = m_transform.GetRotation().GetMatrix();
        instanceData.m_color = m_color;
    }

    ModelInstance& ModelInstance::operator=(const ModelInstance& other)
    {
        if (this == &other)
        {
            return *this;
        }

        m_meshId = other.m_meshId;
        ModelInstanceSystem::Get().UpdateModelInstanceMeshId(
            m_instanceId, m_meshId);

        m_transform = other.m_transform;
        m_color = other.m_color;

        return *this;
    }
} // namespace Star