#include "StarDust/Model/ModelInstance.h"

#include <utility>

namespace Str
{
    ModelInstance::ModelInstance(
        std::string modelName,
        const Uni::Math::Transform& transform,
        const Uni::Grpx::Color& color)
        : m_modelName{ std::move(modelName) }
        , m_transform{ transform }
        , m_color{ color }
    {
    }

    ModelInstance::ModelInstance(
        PrimitiveType primitiveType,
        const Uni::Math::Transform& transform,
        const Uni::Grpx::Color& color)
        : m_primitiveType{ primitiveType }
        , m_transform{ transform }
        , m_color{ color }
    {
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
} // namespace Str