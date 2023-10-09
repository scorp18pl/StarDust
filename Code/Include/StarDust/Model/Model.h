#pragma once

#include <StarDust/Buffer/IndexBuffer.h>
#include <StarDust/Buffer/VertexBuffer.h>
#include <StarDust/Model/Mesh.h>
#include <StarDust/VertexArray.h>
#include <Universal/Math/Matrix/Matrix3x4f.h>

namespace Star
{
    namespace ModelInstance
    {
        struct Data;
    } // namespace ModelInstance

    //! Class used for rendering a mesh.
    class Model
    {
    public:
        explicit Model(Mesh mesh);
        Model(const Model& other);
        Model(Model&& other) noexcept;
        ~Model() = default;

        [[nodiscard]] const Mesh& GetMesh() const;
        Mesh& GetMesh();

        void RenderInstances(std::vector<ModelInstance::Data>& instances);

        Model& operator=(const Model& other);

    private:
        Mesh m_mesh;

        VertexBuffer m_vertexBuffer, m_instanceParametersBuffer;
        IndexBuffer m_indexBuffer;
        VertexArray m_vertexArray;
    };
} // namespace Star
