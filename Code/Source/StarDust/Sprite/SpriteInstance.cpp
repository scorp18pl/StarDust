#include <StarDust/Sprite/SpriteInstance.h>

namespace Star
{

    SpriteInstance::SpriteInstance(
        SpriteAsset::IdType spriteId,
        const Uni::Math::Vector3f& translation,
        float scale,
        float rotationRad)
        : m_instanceId{ ModelInstance::InvalidId }
        , m_spriteId{ spriteId }
        , m_translation{ translation }
        , m_scale{ scale }
        , m_rotation{ rotationRad }
    {
        Mesh::IdType meshId = Mesh::GetId(SpriteAsset::GetMeshName(spriteId));
        m_instanceId = ModelInstanceSystem::Get().RegisterModelInstance(meshId);

        Update();
    }

    SpriteInstance::SpriteInstance(SpriteInstance&& other) noexcept
        : m_instanceId{ other.m_instanceId }
        , m_spriteId{ other.m_spriteId }
        , m_translation{ other.m_translation }
        , m_scale{ other.m_scale }
        , m_rotation{ other.m_rotation }
    {
        other.m_instanceId = ModelInstance::InvalidId;
    }

    SpriteInstance::~SpriteInstance()
    {
        if (m_instanceId != ModelInstance::InvalidId)
        {
            ModelInstanceSystem::Get().UnregisterModelInstance(m_instanceId);
        }
    }

    void SpriteInstance::SetSprite(SpriteAsset::IdType spriteId)
    {
        m_spriteId = spriteId;
        ModelInstanceSystem::Get().UpdateModelInstanceMeshId(
            m_instanceId, Mesh::GetId(SpriteAsset::GetMeshName(spriteId)));
    }

    void SpriteInstance::Update()
    {
        ModelInstance::Data& instanceData =
            ModelInstanceSystem::Get().GetInstanceData(m_instanceId);

        instanceData.m_normalTransform = instanceData.m_transform =
            Uni::Math::Matrix3x4f::CreateTranslation(m_translation) *
            Uni::Math::Matrix3x4f::CreateRotationEulerRadians(m_rotation, Uni::Math::Axis::Z) *
            Uni::Math::Matrix3x4f::CreateScale(Uni::Math::Vector3f{ m_scale });
        instanceData.m_color = Uni::Grpx::Color::White;
    }

    SpriteInstance& SpriteInstance::operator=(const SpriteInstance& other)
    {
        if (this == &other)
        {
            return *this;
        }

        m_spriteId = other.m_spriteId;
        ModelInstanceSystem::Get().UpdateModelInstanceMeshId(m_instanceId, m_spriteId);

        m_translation = other.m_translation;
        m_scale = other.m_scale;
        m_rotation = other.m_rotation;

        return *this;
    }
} // namespace Star
