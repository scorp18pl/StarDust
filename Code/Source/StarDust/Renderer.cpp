#include <StarDust/Model/Mesh.h>
#include <StarDust/Model/ModelRegistry.h>
#include <StarDust/Renderer.h>
#include <StarDust/Shader/ShaderProgramRegistry.h>
#include <StarDust/Utils.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>

namespace Str
{
    Renderer& Renderer::Get()
    {
        static Renderer renderer;
        return renderer;
    }

    int Renderer::RegisterModelInstance(PrimitiveType type)
    {
        if (type == PrimitiveType::None)
        {
            return -1;
        }

        int id;
        if (!m_modelInstances[type].m_freeIds.empty())
        {
            id = m_modelInstances[type].m_freeIds.back();
            m_modelInstances[type].m_freeIds.pop_back();
        }
        else
        {
            id = static_cast<int>(m_modelInstances[type].m_idToDataMap.size());
            m_modelInstances[type].m_idToDataMap.emplace(id, InstanceData());
        }

        return id;
    }

    void Renderer::UnregisterModelInstance(PrimitiveType type, int instanceId)
    {
        if (type == PrimitiveType::None)
        {
            return;
        }

        m_modelInstances[type].m_freeIds.push_back(instanceId);
        m_modelInstances[type].m_idToDataMap.erase(instanceId);
    }

    InstanceData& Renderer::GetInstanceData(PrimitiveType type, int instanceId)
    {
        return m_modelInstances[type].m_idToDataMap[instanceId];
    }

    int Renderer::RegisterLightSource(LightSourceType type)
    {
        int id;
        if (!m_lightSources[type].m_freeIds.empty())
        {
            id = m_lightSources[type].m_freeIds.back();
            m_lightSources[type].m_freeIds.pop_back();
        }
        else
        {
            id = static_cast<int>(m_lightSources[type].m_idToDataMap.size());
            m_lightSources[type].m_idToDataMap.emplace(id, LightData());
        }
        return id;
    }

    void Renderer::UnregisterLightSource(LightSourceType type, int pointLightId)
    {
        m_lightSources[type].m_freeIds.push_back(pointLightId);
        m_lightSources[type].m_idToDataMap.erase(pointLightId);
    }

    LightData& Renderer::GetLightSourceData(
        LightSourceType type, int pointLightId)
    {
        return m_lightSources[type].m_idToDataMap[pointLightId];
    }

    void Renderer::Render()
    {
        m_frameBuffer.Bind();

        GL_CHECK(glViewport(0, 0, m_pixelWidth, m_pixelHeight));

        GL_CHECK(glEnable(GL_DEPTH_TEST));
        GL_CHECK(glClearColor(0.07f, 0.13f, 0.17f, 1.0f));
        GL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        ShaderProgram& instanceShader =
            ShaderProgramRegistry::Get().GetShaderProgram("model_instance");
        instanceShader.Bind();
        UpdatePointLightUniforms(instanceShader, LightSourceType::Point);
        UpdatePointLightUniforms(instanceShader, LightSourceType::Directional);

        for (auto& primitive : m_modelInstances)
        {
            PrimitiveType type = primitive.first;
            std::unordered_map<int, InstanceData>& instances =
                primitive.second.m_idToDataMap;
            m_renderData.reserve(instances.size());
            m_renderData.clear();

            if (instances.empty())
            {
                continue;
            }

            for (auto& instance : instances)
            {
                m_renderData.push_back(instance.second);
            }

            Model& model = ModelRegistry::Get().GetPrimitiveModel(type);
            model.RenderInstances(m_renderData);
        }

        Str::FrameBuffer::Unbind();

        GL_CHECK(glViewport(0, 0, m_windowWidth, m_windowHeight));

        ShaderProgram& postProcShader =
            ShaderProgramRegistry::Get().GetShaderProgram("pixelate");
        postProcShader.Bind();
        postProcShader.SetUniform1i("screenTexture", 0);
        GL_CHECK(glDisable(GL_DEPTH_TEST));
        m_pixelizationTexture.Bind();
        m_frameBuffer.Draw();
    }

    void Renderer::SetPostprocessingFlags(PostProcFlag flags)
    {
        m_postProcFlags = flags;
    }

    void Renderer::SetPixelizationResolution(
        unsigned int width,
        unsigned int height,
        unsigned int windowWidth,
        unsigned int windowHeight)
    {
        m_pixelWidth = width;
        m_pixelHeight = height;

        m_windowWidth = windowWidth;
        m_windowHeight = windowHeight;

        m_pixelizationTexture.SetData(nullptr, width, height);

        m_renderBuffer.SetStorage(GL_DEPTH24_STENCIL8, width, height);

        FrameBuffer::Unbind();
        RenderBuffer::Unbind();
    }

    Renderer::Renderer()
        : m_pixelizationTexture(nullptr, 1, 1)
    {
        m_renderBuffer.SetStorage(GL_DEPTH24_STENCIL8, 1, 1);

        m_frameBuffer.AttachTexture(
            m_pixelizationTexture, GL_COLOR_ATTACHMENT0);
        m_frameBuffer.AttachRenderBuffer(
            m_renderBuffer, GL_DEPTH_STENCIL_ATTACHMENT);
    }

    void Renderer::UpdatePointLightUniforms(
        ShaderProgram& shader, LightSourceType type)
    {
        const std::string lightTypeStr =
            (type == LightSourceType::Point) ? "point" : "directional";

        shader.SetUniform1i(
            std::string("u_" + lightTypeStr + "LightCount").c_str(),
            m_lightSources[type].m_idToDataMap.size());

        int i = 0;
        for (auto& pointLight : m_lightSources[type].m_idToDataMap)
        {
            std::string prefix =
                std::string("u_" + lightTypeStr + "Lights[") +
                std::to_string(i) + "].";
            shader.SetUniform3f(
                (prefix + "vector").c_str(),
                pointLight.second.m_vector.m_x,
                pointLight.second.m_vector.m_y,
                pointLight.second.m_vector.m_z);
            shader.SetUniform4f(
                (prefix + "color").c_str(),
                pointLight.second.m_color.m_x,
                pointLight.second.m_color.m_y,
                pointLight.second.m_color.m_z,
                pointLight.second.m_color.m_w);
            shader.SetUniform1f(
                (prefix + "intensity").c_str(), pointLight.second.m_intensity);
            ++i;
        }
    }
} // namespace Str