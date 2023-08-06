#pragma once

typedef unsigned int GLenum;

namespace Star
{
    class RenderBuffer
    {
    public:
        RenderBuffer();
        RenderBuffer(const RenderBuffer&) = delete;
        RenderBuffer(RenderBuffer&&) = delete;
        ~RenderBuffer();

        void Bind() const;
        static void Unbind();

        [[nodiscard]] unsigned int GetID() const;

        void SetStorage(
            GLenum internalFormat, unsigned int width,
            unsigned int height) const;

        RenderBuffer& operator=(const RenderBuffer&) = delete;
    private:
        unsigned int m_id;
    };
} // namespace Star
