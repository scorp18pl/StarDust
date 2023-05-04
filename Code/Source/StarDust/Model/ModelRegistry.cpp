#include <StarDust/Model/ModelRegistry.h>

namespace Str
{
    ModelRegistry& ModelRegistry::Get()
    {
        static ModelRegistry instance;
        return instance;
    }

    Model& ModelRegistry::GetPrimitiveModel(PrimitiveType type)
    {
        if (m_primitiveModels.find(type) == m_primitiveModels.end())
        {
            m_primitiveModels.emplace(type, GeneratePrimitiveMesh(type));
        }
        return m_primitiveModels.at(type);
    }

    Mesh ModelRegistry::GeneratePrimitiveMesh(PrimitiveType type)
    {
        Mesh mesh;
        switch (type)
        {
        case PrimitiveType::Triangle:
            mesh = GenerateTriangleMesh();
            break;
        case PrimitiveType::Rectangle:
            mesh = GenerateRectangleMesh();
            break;
        case PrimitiveType::Box:
            mesh = GenerateBoxMesh();
            break;
        case PrimitiveType::Icosahedron:
            mesh = GenerateIcosahedronMesh();
            break;
        }

        return mesh;
    }

    Mesh ModelRegistry::GenerateTriangleMesh()
    {
        static const Uni::Math::Matrix3x4f
            VertexRotations[TriangleVertexCount] = {
                Uni::Math::Matrix3x4f::CreateIdentity(),
                Uni::Math::Matrix3x4f::CreateFromRotationDegrees(
                    120.0f, Uni::Math::Axis::X),
                Uni::Math::Matrix3x4f::CreateFromRotationDegrees(
                    240.0f, Uni::Math::Axis::X),
            };

        static const Uni::Math::Vector3f
            TemplatePositions[TriangleVertexCount] = {
                { 0.0f, 0.0f, 1.0f },
                { VertexRotations[1] *
                  Uni::Math::Vector3f{ 0.0f, 0.0f, 1.0f } },
                { VertexRotations[2] *
                  Uni::Math::Vector3f{ 0.0f, 0.0f, 1.0f } },
            };

        static const Uni::Math::Vector3f
            TemplateNormals[TriangleVertexCount] = {
                { 1.0f, 0.0f, 0.0f },
                { 1.0f, 0.0f, 0.0f },
                { 1.0f, 0.0f, 0.0f },
            };

        static const Index TemplateIndices[TriangleIndexCount] = {
            0,
            1,
            2,
        };

        return {
            {
                { TemplatePositions[0], TemplateNormals[0] },
                { TemplatePositions[1], TemplateNormals[1] },
                { TemplatePositions[2], TemplateNormals[2] },
            },
            {
                TemplateIndices[0],
                TemplateIndices[1],
                TemplateIndices[2],
            },
        };
    }

    Mesh ModelRegistry::GenerateRectangleMesh()
    {
        static const Uni::Math::Vector3f
            TemplatePositions[RectangleVertexCount] = {
                { 0.0f, -0.5f, 0.5f }, // Top Left
                { 0.0f, 0.5f, 0.5f }, // Top Right
                { 0.0f, -0.5f, -0.5f }, // Bottom Left
                { 0.0f, 0.5f, -0.5f }, // Bottom Right
            };

        static const Uni::Math::Vector3f
            TemplateNormals[RectangleVertexCount] = {
                { 1.0f, 0.0f, 0.0f },
                { 1.0f, 0.0f, 0.0f },
                { 1.0f, 0.0f, 0.0f },
                { 1.0f, 0.0f, 0.0f },
            };

        static const Uni::Math::Vector2f TemplateUVs[RectangleVertexCount] = {
            { 0.0f, 1.0f },
            { 1.0f, 1.0f },
            { 0.0f, 0.0f },
            { 1.0f, 0.0f },
        };

        static const Index TemplateIndices[RectangleIndexCount] = {
            0, 1, 2, 2, 1, 3,
        };

        return {
            {
                { TemplatePositions[0], TemplateNormals[0], TemplateUVs[0] },
                { TemplatePositions[1], TemplateNormals[1], TemplateUVs[1] },
                { TemplatePositions[2], TemplateNormals[2], TemplateUVs[2] },
                { TemplatePositions[3], TemplateNormals[3], TemplateUVs[3] },
            },
            {
                TemplateIndices[0],
                TemplateIndices[1],
                TemplateIndices[2],
                TemplateIndices[3],
                TemplateIndices[4],
                TemplateIndices[5],
            },
        };
    }

    Mesh ModelRegistry::GenerateBoxMesh()
    {
        static const Mesh& FaceMesh =
            GenerateRectangleMesh(); // TODO - Make it more optimal

        static const std::vector<Uni::Math::Matrix3x4f> FaceRotation = {
            Uni::Math::Matrix3x4f::CreateIdentity(), // Front
            Uni::Math::Matrix3x4f::CreateFromRotationDegrees(
                90.0f, Uni::Math::Axis::Z), // Right
            Uni::Math::Matrix3x4f::CreateFromRotationDegrees(
                180.0f, Uni::Math::Axis::Z), // Back
            Uni::Math::Matrix3x4f::CreateFromRotationDegrees(
                -90.0f, Uni::Math::Axis::Z), // Left
            Uni::Math::Matrix3x4f::CreateFromRotationDegrees(
                -90.0f, Uni::Math::Axis::Y), // Top
            Uni::Math::Matrix3x4f::CreateFromRotationDegrees(
                90.0f, Uni::Math::Axis::Y), // Bottom
        };

        static const Uni::Math::Matrix3x4f translation =
            Uni::Math::Matrix3x4f::CreateFromTranslation({ 0.5f, 0.0f, 0.0f });

        Mesh mesh;
        mesh.m_vertices.reserve(BoxVertexCount);
        mesh.m_indices.reserve(BoxIndexCount);

        for (unsigned int face = 0U; face < BoxFaceCount; ++face)
        {
            const Uni::Math::Matrix3x4f& vertexTransform =
                FaceRotation[face] * translation;
            Uni::Math::Matrix3x4f normalTransform = FaceRotation[face];
            normalTransform(0, 3) = 0.0f;
            normalTransform(1, 3) = 0.0f;
            normalTransform(2, 3) = 0.0f;

            for (unsigned int vertex = 0U; vertex < RectangleVertexCount;
                 ++vertex)
            {
                mesh.m_vertices.push_back({
                    vertexTransform * FaceMesh.m_vertices[vertex].m_position,
                    normalTransform * FaceMesh.m_vertices[vertex].m_normal,
                });
            }

            for (unsigned int index = 0U; index < RectangleIndexCount; ++index)
            {
                mesh.m_indices.push_back(
                    FaceMesh.m_indices[index] + face * RectangleVertexCount);
            }
        }
        return mesh;
    }

    Mesh ModelRegistry::GenerateIcosahedronMesh()
    {
        static constexpr float Scale = 0.6070619982f;
        static constexpr float Distance = 0.79465447229f;
        static constexpr float AngleFace1CenterFace2 = 0.7297276277; // Radians

        static const Uni::Math::Matrix3x4f InitialTransform =
            Uni::Math::Matrix3x4f::CreateFromTranslation(
                { Distance, 0.0f, 0.0f }) *
            Uni::Math::Matrix3x4f::CreateFromScale(
                Uni::Math::Vector3f::CreateFromFloat(Scale));

        static constexpr unsigned int IcosahedronHalfFaceCount =
            IcosahedronFaceCount / 2U;

        static const Uni::Math::Matrix3x4f BottomTransform =
            Uni::Math::Matrix3x4f::CreateFromRotationDegrees(
                90.0f, Uni::Math::Axis::Y) *
            InitialTransform;

        static const Mesh& FaceMesh = GenerateTriangleMesh();

        Mesh mesh;
        mesh.m_vertices.reserve(IcosahedronVertexCount);
        mesh.m_indices.reserve(IcosahedronIndexCount);
        // Create 10 faces of the icosahedron

        static Uni::Math::Matrix3x4f FaceTransforms[IcosahedronFaceCount];
        FaceTransforms[0] = Uni::Math::Matrix3x4f::CreateFromRotationDegrees(
                                90.0f, Uni::Math::Axis::Y) *
            InitialTransform;

        static constexpr unsigned int PhaseOneFaceCount = 1U;
        static constexpr unsigned int PhaseTwoFaceCount = 3U;
        static constexpr unsigned int PhaseThreeFacesPerPhaseTwo = 2U;
        for (unsigned int PhaseTwoFace = 0U; PhaseTwoFace < PhaseTwoFaceCount;
             ++PhaseTwoFace)
        {
            FaceTransforms[PhaseTwoFace + PhaseOneFaceCount] =
                Uni::Math::Matrix3x4f::CreateFromRotationDegrees(
                    static_cast<float>(PhaseTwoFace) * 120.0f,
                    Uni::Math::Axis::Z) *
                Uni::Math::Matrix3x4f::CreateFromRotationRadians(
                    AngleFace1CenterFace2, Uni::Math::Axis::Y) *
                Uni::Math::Matrix3x4f::CreateFromRotationDegrees(
                    180.0f, Uni::Math::Axis::Z);

            for (unsigned int PhaseThreeFace = 0U;
                 PhaseThreeFace < PhaseThreeFacesPerPhaseTwo;
                 ++PhaseThreeFace)
            {
                const unsigned int FaceIndex = PhaseOneFaceCount +
                    PhaseTwoFaceCount +
                    PhaseTwoFace * PhaseThreeFacesPerPhaseTwo + PhaseThreeFace;

                FaceTransforms[FaceIndex] =
                    FaceTransforms[PhaseTwoFace + PhaseOneFaceCount] *
                    Uni::Math::Matrix3x4f::CreateFromRotationDegrees(
                        static_cast<float>(PhaseThreeFace + 1U) * 120.0f,
                        Uni::Math::Axis::Z) *
                    Uni::Math::Matrix3x4f::CreateFromRotationRadians(
                        AngleFace1CenterFace2, Uni::Math::Axis::Y) *
                    Uni::Math::Matrix3x4f::CreateFromRotationDegrees(
                        180.0f, Uni::Math::Axis::Z) *
                    FaceTransforms[0];
            }

            FaceTransforms[PhaseTwoFace + PhaseOneFaceCount] *=
                FaceTransforms[0];
        }

        for (unsigned int PhaseFourFace = 0U;
             PhaseFourFace < IcosahedronHalfFaceCount;
             ++PhaseFourFace)
        {
            FaceTransforms[PhaseFourFace + IcosahedronHalfFaceCount] =
                Uni::Math::Matrix3x4f::CreateFromScale({ 1.0f, 1.0f, -1.0f }) *
                Uni::Math::Matrix3x4f::CreateFromRotationDegrees(
                    60.0f, Uni::Math::Axis::Z) *
                FaceTransforms[PhaseFourFace];
        }

        for (unsigned int Face = 0U; Face < IcosahedronFaceCount; ++Face)
        {
            const Uni::Math::Matrix3x4f& VertexTransform = FaceTransforms[Face];
            Uni::Math::Matrix3x4f normalTransform = VertexTransform;
            normalTransform(0, 3) = 0.0f;
            normalTransform(1, 3) = 0.0f;
            normalTransform(2, 3) = 0.0f;

            for (unsigned int Vertex = 0U; Vertex < TriangleVertexCount;
                 ++Vertex)
            {
                mesh.m_vertices.push_back({
                    VertexTransform * FaceMesh.m_vertices[Vertex].m_position,
                    VertexTransform * FaceMesh.m_vertices[Vertex].m_normal,
                });
            }

            for (unsigned int Index = 0U; Index < TriangleIndexCount; ++Index)
            {
                mesh.m_indices.push_back(
                    FaceMesh.m_indices[Index] + Face * TriangleVertexCount);
            }
        }

        return mesh;
    }
} // namespace Str