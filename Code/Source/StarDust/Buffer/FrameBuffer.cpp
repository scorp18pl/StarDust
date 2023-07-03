#include <StarDust/Buffer/FrameBuffer.h>
#include <StarDust/Buffer/IndexBuffer.h>
#include <StarDust/Buffer/VertexBuffer.h>
#include <StarDust/Buffer/VertexBufferLayout.h>
#include <StarDust/Model/Mesh.h>
#include <StarDust/Texture.h>
#include <StarDust/VertexArray.h>
#include <StarDust/Utilities/Utils.h>
#include <stdexcept>

namespace Star
{
    FrameBuffer::FrameBuffer()
    {
        GL_CHECK(glGenFramebuffers(1, &m_id));
    }

    FrameBuffer::~FrameBuffer()
    {
        GL_CHECK(glDeleteFramebuffers(1, &m_id));
    }

    void FrameBuffer::AttachTexture(
        const Texture& texture, GLenum attachmentType) const
    {
        Bind();
        GL_CHECK(glFramebufferTexture2D(
            GL_FRAMEBUFFER, attachmentType, GL_TEXTURE_2D, texture.GetID(), 0));
    }

    void FrameBuffer::AttachRenderBuffer(
        const RenderBuffer& renderBuffer, GLenum attachmentType) const
    {
        Bind();
        GL_CHECK(glFramebufferRenderbuffer(
            GL_FRAMEBUFFER,
            attachmentType,
            GL_RENDERBUFFER,
            renderBuffer.GetID()));

        auto fbStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (fbStatus != GL_FRAMEBUFFER_COMPLETE)
        {
            throw std::runtime_error(
                "Framebuffer error: " + std::to_string(fbStatus));
        }
    }

    void FrameBuffer::Bind() const
    {
        GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, m_id));
    }

    void FrameBuffer::Unbind()
    {
        GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    }

    void FrameBuffer::Draw()
    {
        static Mesh sqreenQuad;
        static VertexBuffer vertexBuffer;
        static IndexBuffer indexBuffer;
        static VertexArray vertexArray;

        static bool initialized = ([&](){
            sqreenQuad = {
               .m_vertices{
                   {
                        { -1.0f, -1.0f, 0.0f },
                        { 0.0f, 0.0f, 1.0f },
                        { 0.0f, 0.0f }
                   },
                   {
                        {  1.0f, -1.0f, 0.0f },
                        { 0.0f, 0.0f, 1.0f },
                        { 1.0f, 0.0f }
                   },
                   {
                        {  1.0f,  1.0f, 0.0f },
                        { 0.0f, 0.0f, 1.0f },
                        { 1.0f, 1.0f }
                   },
                   {
                        { -1.0f,  1.0f, 0.0f },
                        { 0.0f, 0.0f, 1.0f},
                        { 0.0f, 1.0f}
                   },
                },
                .m_indices{
                     0, 1, 2,
                     2, 3, 0
                }
           };

            vertexBuffer.SetData(
                sqreenQuad.m_vertices.data(),
                sqreenQuad.m_vertices.size() * sizeof(Vertex));
            indexBuffer.SetData(
                sqreenQuad.m_indices.data(), sqreenQuad.m_indices.size());

            VertexBufferLayout layout;
            layout.Push<Uni::Math::Vector3f>(1); // Position
            layout.Push<Uni::Math::Vector3f>(1); // Normal
            layout.Push<float>(2); // Texture coordinates
            vertexArray.AddBuffer(vertexBuffer, layout);

            return true;
        }).operator()();

        vertexArray.Bind();
        indexBuffer.Bind();
        GL_CHECK(glDrawElements(
            GL_TRIANGLES,
            sqreenQuad.m_indices.size(),
            GL_UNSIGNED_INT,
            nullptr));
    }
} // namespace Star