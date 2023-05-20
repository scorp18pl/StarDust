#include <StarDust/Model/Mesh.h>
#include <array>
#include <filesystem>
#include <fstream>
#include <sstream>

namespace Star
{
    struct ObjVertexAttributes
    {
        std::vector<Uni::Math::Vector3f> m_vertexPositions;
        std::vector<Uni::Math::Vector2f> m_vertexTexCoords;
        std::vector<Uni::Math::Vector3f> m_vertexNormals;
    };

    struct VertexComposition
    {
        // -1 means default value
        int m_vertexPositionIndex;
        int m_vertexTexCoordIndex;
        int m_vertexNormalIndex;
    };

    enum class FaceType
    {
        Triangle,
        Quad
    };

    struct Face
    {
        FaceType m_type;
        std::array<VertexComposition, 4U> m_vertices;
    };

    std::istream& operator>>(std::istream& is, Uni::Math::Vector2f& vector)
    {
        is >> vector.m_x >> vector.m_y;
        return is;
    }

    std::istream& operator>>(std::istream& is, Uni::Math::Vector3f& vector)
    {
        is >> vector.m_x >> vector.m_y >> vector.m_z;
        return is;
    }

    std::istream& operator>>(
        std::istream& is, VertexComposition& vertexComposition)
    {
        vertexComposition.m_vertexPositionIndex = -1;
        vertexComposition.m_vertexTexCoordIndex = -1;
        vertexComposition.m_vertexNormalIndex = -1;

        std::string indexString;
        int i = 0;
        while (std::getline(is, indexString, '/'))
        {
            if (!indexString.empty())
            {
                int index = std::stoi(indexString) - 1;
                if (i == 0)
                {
                    vertexComposition.m_vertexPositionIndex = index;
                }
                else if (i == 1)
                {
                    vertexComposition.m_vertexTexCoordIndex = index;
                }
                else if (i == 2)
                {
                    vertexComposition.m_vertexNormalIndex = index;
                }
            }
            ++i;
        }

        return is;
    }

    std::istream& operator>>(std::istream& is, Face& face)
    {
        std::string vertexData;
        int vertexCount = 0;
        while (is >> vertexData)
        {
            VertexComposition vertexComposition;
            std::istringstream vertexDataStream{ vertexData };
            vertexDataStream >> vertexComposition;

            face.m_vertices[vertexCount] = vertexComposition;
            ++vertexCount;

            // Handle the case for triangle and quad
            if (vertexCount == 3)
            {
                face.m_type = FaceType::Triangle;
            }
            else if (vertexCount == 4)
            {
                face.m_type = FaceType::Quad;
                break;
            }
        }

        if (vertexCount < 3)
        {
            throw std::runtime_error{ "Invalid face data" };
        }

        return is;
    }

    Mesh Mesh::ReadFromObjFile(const std::string& fileName)
    {
        Mesh resultMesh;
        std::filesystem::path filePath{ fileName };
        resultMesh.m_name = filePath.stem().string();

        std::ifstream fileStream{ fileName };

        if (!fileStream.is_open())
        {
            throw std::runtime_error{ "Failed to open file: " + fileName };
        }

        ObjVertexAttributes vertexAttributes;
        std::vector<Face> faces;
        std::string line;
        for (std::getline(fileStream, line); !fileStream.eof();
             std::getline(fileStream, line))
        {
            std::istringstream lineStream{ line };
            std::string lineType;
            lineStream >> lineType;

            if (lineType == "v")
            {
                Uni::Math::Vector3f vertexPosition;
                lineStream >> vertexPosition;
                vertexAttributes.m_vertexPositions.push_back(vertexPosition);
            }
            else if (lineType == "vt")
            {
                Uni::Math::Vector2f vertexTexCoord;
                lineStream >> vertexTexCoord;
                vertexAttributes.m_vertexTexCoords.push_back(vertexTexCoord);
            }
            else if (lineType == "vn")
            {
                Uni::Math::Vector3f vertexNormal;
                lineStream >> vertexNormal;

                // Normalize the vertex normal
                vertexNormal = vertexNormal.GetNormalized();

                vertexAttributes.m_vertexNormals.push_back(vertexNormal);
            }
            else if (lineType == "f")
            {
                Face face;
                lineStream >> face;
                faces.push_back(face);
            }
        }

        unsigned int baseIndex = resultMesh.m_indices.size();
        for (const auto& face : faces)
        {
            unsigned int vertexCount =
                face.m_type == FaceType::Triangle ? 3 : 4;
            for (unsigned int i = 0; i < vertexCount; ++i)
            {
                Vertex vertex;
                const VertexComposition& vertexComposition = face.m_vertices[i];

                if (vertexComposition.m_vertexPositionIndex >= 0)
                {
                    vertex.m_position =
                        vertexAttributes.m_vertexPositions
                            [vertexComposition.m_vertexPositionIndex];
                }

                if (vertexComposition.m_vertexTexCoordIndex >= 0)
                {
                    vertex.m_texCoordinates =
                        vertexAttributes.m_vertexTexCoords
                            [vertexComposition.m_vertexTexCoordIndex];
                }

                if (vertexComposition.m_vertexNormalIndex >= 0)
                {
                    vertex.m_normal =
                        vertexAttributes.m_vertexNormals
                            [vertexComposition.m_vertexNormalIndex];
                }

                resultMesh.m_vertices.push_back(vertex);
            }

            if (face.m_type == FaceType::Triangle)
            {
                resultMesh.m_indices.push_back(resultMesh.m_indices.size());
                resultMesh.m_indices.push_back(resultMesh.m_indices.size());
                resultMesh.m_indices.push_back(resultMesh.m_indices.size());
            }
            else if (face.m_type == FaceType::Quad)
            {

                resultMesh.m_indices.push_back(baseIndex);
                resultMesh.m_indices.push_back(baseIndex + 1);
                resultMesh.m_indices.push_back(baseIndex + 2);

                resultMesh.m_indices.push_back(baseIndex);
                resultMesh.m_indices.push_back(baseIndex + 2);
                resultMesh.m_indices.push_back(baseIndex + 3);
            }

            baseIndex += vertexCount;
        }

        return resultMesh;
    }

    void Mesh::WriteToObjFile(const std::string& fileName)
    {
        std::ofstream fileStream{ fileName };

        if (!fileStream.is_open())
        {
            return;
        }

        for (const auto& vertex : m_vertices)
        {
            fileStream << "v " << vertex.m_position.m_x << " "
                       << vertex.m_position.m_y << " " << vertex.m_position.m_z
                       << "\n";
        }

        for (const auto& vertex : m_vertices)
        {
            fileStream << "vt " << vertex.m_texCoordinates.m_x << " "
                       << vertex.m_texCoordinates.m_y << "\n";
        }

        for (const auto& vertex : m_vertices)
        {
            fileStream << "vn " << vertex.m_normal.m_x << " "
                       << vertex.m_normal.m_y << " " << vertex.m_normal.m_z
                       << "\n";
        }

        const unsigned int ComponentCount = 3U;
        for (unsigned int i = 0; i < m_indices.size(); i += ComponentCount)
        {
            fileStream << "f ";
            for (unsigned int j = 0; j < ComponentCount; ++j)
            {
                fileStream << m_indices[i + j] + 1U << "/"
                           << m_indices[i + j] + 1U << "/"
                           << m_indices[i + j] + 1U << " ";
            }
            fileStream << "\n";
        }
    }
} // namespace Star
