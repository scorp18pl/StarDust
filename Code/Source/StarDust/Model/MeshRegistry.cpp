#include <StarDust/Model/MeshRegistry.h>
#include <StarDust/Utils.h>
#include <filesystem>

namespace Star
{
    MeshRegistry& MeshRegistry::Get()
    {
        static MeshRegistry instance;
        return instance;
    }

    const std::unordered_map<Mesh::IdType, Mesh>& MeshRegistry::
        GetRegisteredMeshes() const
    {
        return m_registeredMeshes;
    }

    Mesh::IdType MeshRegistry::GetMeshId(const std::string& name) const
    {
        for (const auto& [id, mesh] : m_registeredMeshes)
        {
            if (mesh.m_name == name)
            {
                return id;
            }
        }

        return Mesh::InvalidId;
    }

    const Mesh& MeshRegistry::GetMesh(Mesh::IdType id) const
    {
        return m_registeredMeshes.at(id);
    }

    Mesh& MeshRegistry::GetMesh(Mesh::IdType id)
    {
        return m_registeredMeshes.at(id);
    }

    Mesh::IdType MeshRegistry::RegisterMesh(const Mesh& mesh)
    {
        if (m_freeIds.empty())
        {
            m_registeredMeshes[m_registeredMeshes.size()] = mesh;
            return m_registeredMeshes.size() - 1;
        }
        else
        {
            const Mesh::IdType id = m_freeIds.back();
            m_freeIds.pop_back();
            m_registeredMeshes[id] = mesh;
            return id;
        }
    }

    void MeshRegistry::UnregisterMesh(Mesh::IdType id)
    {
        m_registeredMeshes.erase(id);
        m_freeIds.push_back(id);
    }

    MeshRegistry::MeshRegistry()
    {
        static const std::filesystem::path meshPath =
            Utils::GetResourcesPath() / "Meshes";

        for (const auto& entry : std::filesystem::directory_iterator(meshPath))
        {
            if (entry.is_regular_file() && entry.path().extension() == ".obj")
            {
                const std::string& path = entry.path().string();
                RegisterMesh(Mesh::ReadFromObjFile(path));
            }
        }
    }
} // namespace Star