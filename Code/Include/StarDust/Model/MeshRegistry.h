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

        const Mesh& GetMesh(Mesh::IdType id) const { return m_registeredMeshes.at(id); }
        const Mesh& GetMesh(const std::string& name) const { return GetMesh(Mesh::GetId(name)); }

        Mesh& GetMesh(Mesh::IdType id) { return m_registeredMeshes.at(id); }
        Mesh& GetMesh(const std::string& name) { return GetMesh(Mesh::GetId(name)); }

        void RegisterMesh(const Mesh& mesh);
        void UnregisterMesh(Mesh::IdType id);
        void UnregisterMesh(const std::string& name) { UnregisterMesh(Mesh::GetId(name)); }

    private:
        MeshRegistry();

        std::unordered_map<Mesh::IdType, Mesh> m_registeredMeshes;
    };
} // namespace Star
