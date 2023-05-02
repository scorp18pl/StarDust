#include <StarDust/Drawable/Drawable.h>
#include <StarDust/Renderer.h>
#include <StarDust/Utils.h>
#include <cstring>

namespace Str
{
    Renderer::Renderer()
        : m_drawQueue{}
        , m_cachedVertexCount{ 0LU }
        , m_cachedIndexCount{ 0LU }
    {
    }

    void Renderer::AddDrawRequest(Drawable* drawable)
    {
        const auto& Vertices = drawable->GetVertices();
        const auto& Indices = drawable->GetIndices();

        m_cachedVertexCount += Vertices.size();
        m_cachedIndexCount += Indices.size();

        m_drawQueue.push_back({
            Vertices,
            Indices,
        });
    }

    void Renderer::RenderBatched()
    {
        static VertexBuffer vertexBuffer;
        static IndexBuffer indexBuffer;
        static VertexArray vertexArray = ([&]() {
            Str::VertexBufferLayout layout;
            layout.Push<float>(3);
            layout.Push<float>(4);
            layout.Push<float>(2);

            VertexArray vertexArray;
            vertexArray.AddBuffer(vertexBuffer, layout);

            return vertexArray;
        }).operator()();

        m_vertices.resize(m_cachedVertexCount);
        m_indices.resize(m_cachedIndexCount);

        size_t currentVertexCount = 0LU;
        size_t currentIndexCount = 0LU;
        for (const auto& request : m_drawQueue)
        {
            std::copy(
                request.m_vertices.begin(),
                request.m_vertices.end(),
                m_vertices.begin() + currentVertexCount);

            std::copy(
                request.m_indices.begin(),
                request.m_indices.end(),
                m_indices.begin() + currentIndexCount);

            for (size_t index = 0LU; index < request.m_indices.size(); ++index)
            {
                m_indices[currentIndexCount + index] += currentVertexCount;
            }

            currentVertexCount += request.m_vertices.size();
            currentIndexCount += request.m_indices.size();
        }

        vertexBuffer.SetData(
            m_vertices.data(), m_vertices.size() * sizeof(Vertex));
        indexBuffer.SetData(
            m_indices.data(), m_indices.size());

        m_drawQueue.clear();
        m_cachedVertexCount = 0LU;
        m_cachedIndexCount = 0LU;

        indexBuffer.Bind();
        vertexBuffer.Bind();
        vertexArray.Bind();
        GL_CHECK(glDrawElements(
            GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, nullptr));
    }
} // namespace Str