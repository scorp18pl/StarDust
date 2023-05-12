#pragma once

#include <Universal/Graphics/Color.h>
#include <Universal/Math/Vector/Vector3f.h>

namespace Str
{
    enum class LightSourceType
    {
        Point,
        Directional,
    };

    struct LightData
    {
        //! Direction for directional light, position for point light
        Uni::Math::Vector3f m_vector{ 0.0f, 0.0f, 0.0f };
        Uni::Grpx::Color m_color{ 1.0f, 1.0f, 1.0f, 1.0f };
        float m_intensity{ 1.0f };
    };
} // namespace Str
