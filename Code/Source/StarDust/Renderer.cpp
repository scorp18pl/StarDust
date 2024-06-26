#include <StarDust/Model/ModelInstanceSystem.h>
#include <StarDust/Renderer.h>
#include <StarDust/Shader/ShaderProgramRegistry.h>
#include <StarDust/Utilities/Utils.h>
#include <glad/glad.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>

namespace Star
{
    ShaderProgram& Renderer::GetUsedShaderProgram() const
    {
        return ShaderProgramRegistry::Get().GetShaderProgram(
            m_optionFlags & OptionFlag::Lighting ? "model_instance_lighting" : "model_instance");
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

    LightData& Renderer::GetLightSourceData(LightSourceType type, int pointLightId)
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

        if (m_optionFlags & OptionFlag::Lighting)
        {
            ShaderProgram& instanceShader = GetUsedShaderProgram();

            UpdatePointLightUniforms(instanceShader, LightSourceType::Point);
            UpdatePointLightUniforms(instanceShader, LightSourceType::Directional);
        }

        ModelInstanceSystem::Get().Render();
        FrameBuffer::Unbind();

        GL_CHECK(glViewport(0, 0, m_windowWidth, m_windowHeight));

        ShaderProgram& postProcShader = ShaderProgramRegistry::Get().GetShaderProgram("pixelate");
        postProcShader.Bind();
        postProcShader.SetUniform1i("screenTexture", 0);
        GL_CHECK(glDisable(GL_DEPTH_TEST));
        m_pixelizationTexture.Bind();
        FrameBuffer::Draw();
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

        m_frameBuffer.AttachTexture(m_pixelizationTexture, GL_COLOR_ATTACHMENT0);
        m_frameBuffer.AttachRenderBuffer(m_renderBuffer, GL_DEPTH_STENCIL_ATTACHMENT);
    }

    void Renderer::UpdatePointLightUniforms(ShaderProgram& shader, LightSourceType type)
    {
        const std::string lightTypeStr = (type == LightSourceType::Point) ? "point" : "directional";

        shader.SetUniform1i(
            std::string("u_" + lightTypeStr + "LightCount").c_str(),
            m_lightSources[type].m_idToDataMap.size());

        int i = 0;
        for (auto& pointLight : m_lightSources[type].m_idToDataMap)
        {
            std::string prefix =
                std::string("u_" + lightTypeStr + "Lights[") + std::to_string(i) + "].";
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
            shader.SetUniform1f((prefix + "intensity").c_str(), pointLight.second.m_intensity);
            ++i;
        }
    }
} // namespace Star