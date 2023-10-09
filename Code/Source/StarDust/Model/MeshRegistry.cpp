#include <StarDust/Model/MeshRegistry.h>
#include <StarDust/Utilities/Utils.h>
#include <filesystem>

namespace Star
{
    MeshRegistry& MeshRegistry::Get()
    {
        static MeshRegistry instance;
        return instance;
    }

    const std::unordered_map<Mesh::IdType, Mesh>& MeshRegistry::GetRegisteredMeshes() const
    {
        return m_registeredMeshes;
    }

    void MeshRegistry::RegisterMesh(const Mesh& mesh)
    {
        m_registeredMeshes[mesh.GetId()] = mesh;
    }

    void MeshRegistry::UnregisterMesh(Mesh::IdType id) { m_registeredMeshes.erase(id); }

    MeshRegistry::MeshRegistry()
    {
        static const std::filesystem::path meshPath = Utils::GetResourcesPath() / "Meshes";

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