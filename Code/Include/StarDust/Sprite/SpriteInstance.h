#pragma once

#include <StarDust/Model/Model.h>
#include <StarDust/Model/ModelInstanceSystem.h>
#include <StarDust/Sprite/SpriteAsset.h>
#include <string>

namespace Star
{
    class SpriteInstance
    {
    public:
        static SpriteInstance CreateSpriteInstance(
            const std::string& spriteName,
            const Uni::Math::Vector3f& translation = Uni::Math::Vector3f::CreateZero(),
            float scale = 1.0f,
            float rotationRad = 0.0f);

        static SpriteInstance CreateSubSpriteInstance(
            const std::string& spriteName,
            const std::string& subspriteName,
            const Uni::Math::Vector3f& translation = Uni::Math::Vector3f::CreateZero(),
            float scale = 1.0f,
            float rotationRad = 0.0f);

        SpriteInstance(const SpriteInstance& other) { (*this) = other; }
        SpriteInstance(SpriteInstance&&) noexcept;
        ~SpriteInstance();

        [[nodiscard]] SpriteAsset::IdType GetSpriteId() const { return m_spriteId; }
        [[nodiscard]] const Uni::Math::Vector3f& GetTranslation() const { return m_translation; }
        [[nodiscard]] float GetScale() const { return m_scale; }
        [[nodiscard]] float GetRotation() const { return m_rotation; }

        void SetTranslation(const Uni::Math::Vector3f& translation) { m_translation = translation; }
        void SetScale(float scale) { m_scale = scale; }
        void SetRotation(float rotation) { m_rotation = rotation; }

        void SetSprite(const std::string& spriteName);

        void Update();

        SpriteInstance& operator=(const SpriteInstance&);

    private:
        SpriteInstance(
            const std::string& spriteMeshName,
            const Uni::Math::Vector3f& translation = Uni::Math::Vector3f::CreateZero(),
            float scale = 1.0f,
            float rotationRad = 0.0f);

        ModelInstance::IdType m_instanceId{ ModelInstance::InvalidId };
        SpriteAsset::IdType m_spriteId{ SpriteAsset::InvalidId };

        Uni::Math::Vector3f m_translation{ Uni::Math::Vector3f::CreateZero() };
        float m_scale{ 1.0f };
        float m_rotation{ 0.0f };
    };
} // namespace Star
