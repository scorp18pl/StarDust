#pragma once

#include <StarDust/Buffer/IndexBuffer.h>
#include <StarDust/Buffer/VertexBuffer.h>
#include <StarDust/Model/Mesh.h>
#include <StarDust/VertexArray.h>
#include <Universal/Math/Matrix/Matrix3x4f.h>

namespace Str
{
    struct InstanceData;

    //! Class used for rendering a mesh.
    class Model
    {
    public:
        explicit Model(Mesh mesh);
        Model(const Model& other) = delete;
        Model(Model&& other) noexcept = delete;
        ~Model() = default;

        Mesh GetMesh() const;

        void RenderInstances(
            std::vector<InstanceData>& instances);

        Model& operator=(const Model& other) = delete;

    private:
        Mesh m_mesh;

        VertexBuffer m_vertexBuffer, m_instanceParametersBuffer;
        IndexBuffer m_indexBuffer;
        VertexArray m_vertexArray;
    };
} // namespace Str
