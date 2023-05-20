#pragma once

#include <Universal/Graphics/Color.h>
#include <Universal/Math/Vector/Vector3f.h>

namespace Star
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

    class LightSource
    {
        using IdType = int;
        static constexpr IdType InvalidId = -1;

    public:
        explicit LightSource(LightSourceType type, LightData lightData = {});
        ~LightSource();

        [[nodiscard]] LightSourceType GetType() const;
        [[nodiscard]] const LightData& GetData() const;

        void UpdateType(LightSourceType type);
        LightData& GetData();

        void Update();

    private:
        IdType m_id{ InvalidId };
        LightSourceType m_type;
        LightData m_data;
    };
} // namespace Star
