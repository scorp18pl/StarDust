#include <StarDust/LightSource.h>
#include <StarDust/Renderer.h>

namespace Star
{

    LightSource::LightSource(
        Star::LightSourceType type, Star::LightData lightData)
        : m_type(type)
        , m_data(lightData)
    {
        m_id = Renderer::Get().RegisterLightSource(type);
        Renderer::Get().GetLightSourceData(type, m_id) = lightData;
    }

    LightSource::~LightSource()
    {
        Renderer::Get().UnregisterLightSource(m_type, m_id);
    }

    LightSourceType LightSource::GetType() const
    {
        return m_type;
    }

    const LightData& LightSource::GetData() const
    {
        return m_data;
    }

    void LightSource::UpdateType(LightSourceType type)
    {
        Renderer::Get().UnregisterLightSource(m_type, m_id);
        m_type = type;
        m_id = Renderer::Get().RegisterLightSource(type);
    }

    LightData& LightSource::GetData()
    {
        return m_data;
    }

    void LightSource::Update()
    {
        Renderer::Get().GetLightSourceData(m_type, m_id) = m_data;
    }

} // namespace Star
