#pragma once

#include <StarDust/Buffer/RenderBuffer.h>
#include <StarDust/Texture.h>

namespace Star
{
    class FrameBuffer
    {
    public:
        FrameBuffer();
        FrameBuffer(const FrameBuffer&) = delete;
        FrameBuffer(FrameBuffer&&) = delete;
        ~FrameBuffer();

        void Bind() const;
        static void Unbind();

        void AttachTexture(const Texture& texture, GLenum attachmentType) const;
        void AttachRenderBuffer(
            const RenderBuffer& renderBuffer, GLenum attachmentType) const;

        static void Draw();

        FrameBuffer& operator=(const FrameBuffer&) = delete;

    private:
        unsigned int m_id;
    };
} // namespace Star
