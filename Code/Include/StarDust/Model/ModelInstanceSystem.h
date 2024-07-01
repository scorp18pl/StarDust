#pragma once

#include <StarDust/Model/Mesh.h>
#include <StarDust/Model/Model.h>
#include <Universal/Graphics/Color.h>
#include <Universal/Math/Matrix/Matrix3x4f.h>
#include <set>
#include <unordered_map>

namespace Star
{
    namespace ModelInstance
    {
        struct Data
        {
            Uni::Math::Matrix3x4f m_transform;
            Uni::Math::Matrix3x4f m_normalTransform;
            Uni::Grpx::Color m_color;
        };

        using IdType = int;
        static constexpr IdType InvalidId = -1;
    } // namespace ModelInstance

    class ModelInstanceSystem
    {
    public:
        static ModelInstanceSystem& Get();

        ModelInstance::IdType RegisterModelInstance(Mesh::IdType meshId);
        void UpdateModelInstanceMeshId(ModelInstance::IdType id, Mesh::IdType meshId);
        void UpdateMeshModel(const Mesh& mesh);
        void UnregisterModelInstance(ModelInstance::IdType id);

        ModelInstance::Data& GetInstanceData(ModelInstance::IdType id);

        void Render();

    private:
        ModelInstanceSystem();

        struct InstanceWrapper
        {
            Mesh::IdType m_meshId{ Mesh::InvalidId };
            ModelInstance::Data m_data;
        };

        struct ModelWrapper
        {
            Model m_model;
            std::set<ModelInstance::IdType> m_instances;
        };

        std::unordered_map<Mesh::IdType, ModelWrapper> m_models;
        std::unordered_map<ModelInstance::IdType, InstanceWrapper> m_instances;
        std::vector<ModelInstance::IdType> m_freeIds;

        std::vector<ModelInstance::Data> m_renderData;
    };
} // namespace Star
