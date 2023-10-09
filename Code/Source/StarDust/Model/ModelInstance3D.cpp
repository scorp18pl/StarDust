#include <StarDust/Model/ModelInstance3D.h>
#include <StarDust/Model/ModelInstanceSystem.h>

namespace Star
{
    ModelInstance3D::ModelInstance3D(
        Mesh::IdType meshId, const Uni::Math::Transform& transform, const Uni::Grpx::Color& color)
        : m_instanceId{ ModelInstanceSystem::Get().RegisterModelInstance(meshId) }
        , m_meshId{ meshId }
        , m_transform{ transform }
        , m_color{ color }
    {
        ModelInstance::Data& instanceData =
            ModelInstanceSystem::Get().GetInstanceData(m_instanceId);

        instanceData.m_transform = m_transform.GetMatrix();
        instanceData.m_normalTransform = m_transform.GetRotation().GetMatrix();
        instanceData.m_color = m_color;
    }

    ModelInstance3D::ModelInstance3D(const ModelInstance3D& other) { (*this) = other; }

    ModelInstance3D::ModelInstance3D(ModelInstance3D&& other) noexcept
        : m_instanceId{ other.m_instanceId }
        , m_meshId{ other.m_meshId }
        , m_transform{ other.m_transform }
        , m_color{ other.m_color }
    {
        other.m_instanceId = ModelInstance::InvalidId;
    }

    ModelInstance3D::~ModelInstance3D()
    {
        if (m_instanceId != ModelInstance::InvalidId)
        {
            ModelInstanceSystem::Get().UnregisterModelInstance(m_instanceId);
        }
    }

    void ModelInstance3D::SetMeshId(Mesh::IdType meshId)
    {
        m_meshId = meshId;
        ModelInstanceSystem::Get().UpdateModelInstanceMeshId(m_instanceId, m_meshId);
    }

    void ModelInstance3D::Update()
    {
        ModelInstance::Data& instanceData =
            ModelInstanceSystem::Get().GetInstanceData(m_instanceId);

        const Uni::Math::Transform& WorldTransform = m_transform.GetWorldTransform();

        instanceData.m_transform = WorldTransform.GetMatrix();
        instanceData.m_normalTransform = WorldTransform.GetRotation().GetMatrix();
        instanceData.m_color = m_color;
    }

    ModelInstance3D& ModelInstance3D::operator=(const ModelInstance3D& other)
    {
        if (this == &other)
        {
            return *this;
        }

        m_meshId = other.m_meshId;
        ModelInstanceSystem::Get().UpdateModelInstanceMeshId(m_instanceId, m_meshId);

        m_transform = other.m_transform;
        m_color = other.m_color;

        return *this;
    }
} // namespace Star