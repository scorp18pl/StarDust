#pragma once

#include <StarDust/Buffer/IndexBuffer.h>
#include <StarDust/Shader.h>
#include <StarDust/Vertex.h>
#include <StarDust/VertexArray.h>

namespace Str
{
    class Drawable;

    struct DrawRequest
    {
        const VertexList& m_vertices;
        const IndexList& m_indices;
    };

    class Renderer
    {
    public:
        Renderer();

        void AddDrawRequest(Drawable* drawable);
        void RenderBatched();

    private:
        std::vector<Vertex> m_vertices;
        std::vector<unsigned int> m_indices;

        std::vector<DrawRequest> m_drawQueue;
        size_t m_cachedVertexCount{ 0LU };
        size_t m_cachedIndexCount{ 0LU };
    };
} // namespace Str
