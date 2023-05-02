#pragma once

#include <glad/glad.h>
#include <StarDust/Buffer/Buffer.h>

namespace Str
{
    class FrameBuffer : public Buffer<GL_FRAMEBUFFER>
    {
    public:
        FrameBuffer();
        FrameBuffer(const FrameBuffer&) = delete;
        FrameBuffer(FrameBuffer&&) = delete;

        FrameBuffer& operator=(const FrameBuffer&) = delete;
    };
} // namespace Str
