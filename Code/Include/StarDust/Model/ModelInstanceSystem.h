#pragma once

#include <StarDust/Model/ModelInstance.h>
#include <set>
#include <unordered_map>

namespace Star
{
    class ModelInstanceSystem
    {
    public:
        static ModelInstanceSystem& Get();

        ModelInstance::IdType RegisterModelInstance(Mesh::IdType meshId);
        void UpdateModelInstanceMeshId(
            ModelInstance::IdType id, Mesh::IdType meshId);
        void UnregisterModelInstance(ModelInstance::IdType id);

        ModelInstanceData& GetInstanceData(ModelInstance::IdType id);

        void Render();

    private:
        ModelInstanceSystem();

        struct InstanceWrapper
        {
            Mesh::IdType m_meshId{ Mesh::InvalidId };
            ModelInstanceData m_data;
        };

        struct ModelWrapper
        {
            Model m_model;
            std::set<ModelInstance::IdType> m_instances;
        };

        std::unordered_map<Mesh::IdType, ModelWrapper> m_models;
        std::unordered_map<ModelInstance::IdType, InstanceWrapper> m_instances;
        std::vector<ModelInstance::IdType> m_freeIds;

        std::vector<ModelInstanceData> m_renderData;
    };
} // namespace Star
