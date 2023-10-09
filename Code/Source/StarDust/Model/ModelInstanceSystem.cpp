#include <StarDust/Model/MeshRegistry.h>
#include <StarDust/Model/Model.h>
#include <StarDust/Model/ModelInstanceSystem.h>
#include <stdexcept>

namespace Star
{
    ModelInstanceSystem& ModelInstanceSystem::Get()
    {
        static ModelInstanceSystem instance;
        return instance;
    }

    ModelInstance::IdType ModelInstanceSystem::RegisterModelInstance(Mesh::IdType meshId)
    {
        ModelInstance::IdType id = ModelInstance::InvalidId;
        if (m_freeIds.empty())
        {
            id = static_cast<ModelInstance::IdType>(m_instances.size());
        }
        else
        {
            id = m_freeIds.back();
            m_freeIds.pop_back();
        }

        m_instances[id].m_meshId = meshId;
        try
        {
            m_models.at(meshId).m_instances.insert(id);
        } catch (const std::out_of_range&)
        {
            const auto& mesh = MeshRegistry::Get().GetRegisteredMeshes().find(meshId);
            if (mesh == MeshRegistry::Get().GetRegisteredMeshes().end())
            {
                throw std::runtime_error("Mesh with id " + std::to_string(meshId) + " not found");
            }
            else
            {
                m_models.emplace(meshId, ModelWrapper{ Model{ mesh->second }, { id } });
            }
        }

        return id;
    }

    void ModelInstanceSystem::UpdateModelInstanceMeshId(
        ModelInstance::IdType id, Mesh::IdType meshId)
    {
        const Mesh::IdType OldMeshId = m_instances[id].m_meshId;
        m_instances[id].m_meshId = meshId;

        m_models.at(OldMeshId).m_instances.erase(id);
        m_models.at(meshId).m_instances.insert(id);
    }

    void ModelInstanceSystem::UnregisterModelInstance(ModelInstance::IdType id)
    {
        m_models.at(m_instances[id].m_meshId).m_instances.erase(id);
        m_instances.erase(id);
        m_freeIds.push_back(id);
    }

    ModelInstance::Data& ModelInstanceSystem::GetInstanceData(ModelInstance::IdType id)
    {
        return m_instances[id].m_data;
    }

    void ModelInstanceSystem::Render()
    {
        for (auto& [meshId, model] : m_models)
        {
            m_renderData.clear();
            m_renderData.reserve(model.m_instances.size());

            for (auto& instanceId : model.m_instances)
            {
                m_renderData.push_back(m_instances[instanceId].m_data);
            }

            if (m_renderData.empty())
            {
                continue;
            }

            model.m_model.RenderInstances(m_renderData);
        }
    }

    ModelInstanceSystem::ModelInstanceSystem()
    {
        const auto& registeredMeshes = MeshRegistry::Get().GetRegisteredMeshes();
        for (auto& [meshId, mesh] : registeredMeshes)
        {
            m_models.emplace(meshId, ModelWrapper{ Model{ mesh }, {} });
        }
    }
} // namespace Star