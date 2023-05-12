#pragma once

#include <StarDust/Buffer/FrameBuffer.h>
#include <StarDust/Buffer/RenderBuffer.h>
#include <StarDust/LightSource.h>
#include <StarDust/Model/ModelInstance.h>
#include <StarDust/Shader/ShaderProgram.h>
#include <StarDust/Texture.h>
#include <unordered_map>

namespace Str
{
    enum class PostProcFlag : uint8_t
    {
        None = 0,
        Pixelized = 1 << 0,
    };

    class Renderer
    {
    public:
        static Renderer& Get();

        int RegisterModelInstance(PrimitiveType type);
        void UnregisterModelInstance(PrimitiveType type, int instanceId);
        InstanceData& GetInstanceData(PrimitiveType type, int instanceId);

        int RegisterLightSource(LightSourceType type);
        void UnregisterLightSource(LightSourceType type, int pointLightId);
        LightData& GetLightSourceData(LightSourceType type, int pointLightId);

        void Render();

        void SetPostprocessingFlags(PostProcFlag flags = PostProcFlag::None);
        void SetPixelizationResolution(
            unsigned int width,
            unsigned int height,
            unsigned int windowWidth,
            unsigned int windowHeight);

    private:
        struct InstanceSystem
        {
            std::unordered_map<int, InstanceData> m_idToDataMap;
            std::vector<int> m_freeIds;
        };

        struct PointLightSystem
        {
            std::unordered_map<int, LightData> m_idToDataMap;
            std::vector<int> m_freeIds;
        };

        std::unordered_map<PrimitiveType, InstanceSystem> m_modelInstances;
        std::unordered_map<LightSourceType, PointLightSystem> m_lightSources;

        std::vector<InstanceData> m_renderData;

        PostProcFlag m_postProcFlags = PostProcFlag::None;

        unsigned int m_pixelWidth = 0;
        unsigned int m_pixelHeight = 0;

        unsigned int m_windowWidth = 0;
        unsigned int m_windowHeight = 0;

        FrameBuffer m_frameBuffer;
        RenderBuffer m_renderBuffer;
        Texture m_pixelizationTexture;

        Renderer();

        void UpdatePointLightUniforms(
            ShaderProgram& shader, LightSourceType type);
    };
} // namespace Str
