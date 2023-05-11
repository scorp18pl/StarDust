#pragma once

#include <StarDust/Model/Model.h>
#include <Universal/Math/Transform.h>
#include <Universal/Math/Vector/Vector3f.h>
#include <string>
#include <unordered_map>

namespace Str
{
    class ModelRegistry
    {
    public:
        static ModelRegistry& Get();
        Model& GetPrimitiveModel(PrimitiveType type);
        static std::string GetPrimitiveModelName(PrimitiveType type);

    private:
        ModelRegistry() = default;

        static constexpr unsigned int TriangleVertexCount = 3U;
        static constexpr unsigned int TriangleIndexCount = 3U;

        static constexpr unsigned int RectangleVertexCount = 4U;
        static constexpr unsigned int RectangleIndexCount = 6U;

        static constexpr unsigned int BoxFaceCount = 6U;
        static constexpr unsigned int BoxVertexCount =
            BoxFaceCount * RectangleVertexCount;
        static constexpr unsigned int BoxIndexCount =
            BoxFaceCount * RectangleIndexCount;

        static constexpr unsigned int IcosahedronFaceCount = 20U;
        static constexpr unsigned int IcosahedronVertexCount =
            IcosahedronFaceCount * TriangleVertexCount;
        static constexpr unsigned int IcosahedronIndexCount =
            IcosahedronFaceCount * TriangleIndexCount;

        std::unordered_map<PrimitiveType, Model> m_primitiveModels;

        static Mesh GeneratePrimitiveMesh(PrimitiveType type);

        // Each generation function is run only once.
        static Mesh GenerateTriangleMesh();
        static Mesh GenerateRectangleMesh();
        static Mesh GenerateBoxMesh();
        static Mesh GenerateIcosahedronMesh();
    };
} // namespace Str
