#pragma once

#include <StarDust/Model/Mesh.h>
#include <string>
#include <unordered_map>

namespace Star
{
    class MeshRegistry
    {
    public:
        static MeshRegistry& Get();

        const std::unordered_map<Mesh::IdType, Mesh>& GetRegisteredMeshes() const;

        Mesh::IdType GetMeshId(const std::string& name) const;

        const Mesh& GetMesh(Mesh::IdType id) const;
        Mesh& GetMesh(Mesh::IdType id);

        Mesh::IdType RegisterMesh(const Mesh& mesh);
        void UnregisterMesh(Mesh::IdType id);

    private:
        MeshRegistry();

        std::unordered_map<Mesh::IdType, Mesh> m_registeredMeshes;
        std::vector<Mesh::IdType> m_freeIds;
    };
} // namespace Star
