#include <StarDust/Model/Model.h>
#include <StarDust/Model/ModelInstance.h>

namespace Str
{
    Str::Model::Model(Mesh mesh)
        : m_mesh{ std::move(mesh) }
    {
        VertexBufferLayout defaultLayout;
        defaultLayout.Push<Uni::Math::Vector3f>(1); // Position
        defaultLayout.Push<Uni::Math::Vector3f>(1); // Normal
        defaultLayout.Push<float>(2); // Texture coordinates

        VertexBufferLayout instanceLayout;
        instanceLayout.Push<Uni::Math::Vector4f>(1); // modelMatrixRow0
        instanceLayout.Push<Uni::Math::Vector4f>(1); // modelMatrixRow1
        instanceLayout.Push<Uni::Math::Vector4f>(1); // modelMatrixRow2
        instanceLayout.Push<Uni::Math::Vector4f>(1); // modelMatrixRow0
        instanceLayout.Push<Uni::Math::Vector4f>(1); // modelMatrixRow1
        instanceLayout.Push<Uni::Math::Vector4f>(1); // modelMatrixRow2
        instanceLayout.Push<Uni::Math::Vector4f>(1); // color

        m_vertexBuffer.SetData(
            m_mesh.m_vertices.data(),
            m_mesh.m_vertices.size() * sizeof(Vertex));
        m_indexBuffer.SetData(m_mesh.m_indices.data(), m_mesh.m_indices.size());

        m_vertexArray.AddBuffer(m_vertexBuffer, defaultLayout);
        m_vertexArray.AddBuffer(m_instanceParametersBuffer, instanceLayout, 3);
    }

    Mesh Model::GetMesh() const
    {
        return m_mesh;
    }

    void Model::RenderInstances(std::vector<InstanceData>& instances)
    {
        m_instanceParametersBuffer.SetData(
            instances.data(), instances.size() * sizeof(InstanceData));

        m_vertexArray.Bind();
        m_indexBuffer.Bind();
        glDrawElementsInstanced(
            GL_TRIANGLES,
            m_mesh.m_indices.size(),
            GL_UNSIGNED_INT,
            nullptr,
            instances.size());
    }
} // namespace Str