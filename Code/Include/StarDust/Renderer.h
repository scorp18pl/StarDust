#pragma once

#include <StarDust/Buffer/FrameBuffer.h>
#include <StarDust/Buffer/RenderBuffer.h>
#include <StarDust/Model/ModelInstance.h>
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

        void AddDrawRequest(const ModelInstance& primitiveInstance);
        void Render();

        void SetPostprocessingFlags(PostProcFlag flags = PostProcFlag::None);
        void SetPixelizationResolution(
            unsigned int width,
            unsigned int height,
            unsigned int windowWidth,
            unsigned int windowHeight);

    private:
        std::unordered_map<PrimitiveType, std::vector<InstanceData>>
            m_drawQueue;

        PostProcFlag m_postProcFlags = PostProcFlag::None;

        unsigned int m_pixelWidth = 0;
        unsigned int m_pixelHeight = 0;

        unsigned int m_windowWidth = 0;
        unsigned int m_windowHeight = 0;

        FrameBuffer m_frameBuffer;
        RenderBuffer m_renderBuffer;
        Texture m_pixelizationTexture;

        Renderer();
    };
} // namespace Str
