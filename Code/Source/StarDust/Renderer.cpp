#include <StarDust/Model/Mesh.h>
#include <StarDust/Model/ModelRegistry.h>
#include <StarDust/Renderer.h>
#include <StarDust/Shader/ShaderProgramRegistry.h>
#include <StarDust/Utils.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>

namespace Str
{
    void Renderer::AddDrawRequest(const ModelInstance& primitiveInstance)
    {
        PrimitiveType type = primitiveInstance.GetPrimitiveType();

        if (type == PrimitiveType::None)
        {
            return;
        }

        Uni::Math::Transform globalTransform =
            primitiveInstance.GetTransform().GetWorldTransform();

        m_drawQueue[type].push_back({
            globalTransform.GetMatrix(),
            globalTransform.GetRotation(),
            primitiveInstance.GetColor(),
        });
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

        for (auto& primitive : m_drawQueue)
        {
            PrimitiveType type = primitive.first;
            std::vector<InstanceData>& instances = primitive.second;

            if (instances.empty())
            {
                continue;
            }

            Model& model = ModelRegistry::Get().GetPrimitiveModel(type);

            model.RenderInstances(instances);
        }

        m_drawQueue.clear();

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

    Renderer& Renderer::Get()
    {
        static Renderer renderer;
        return renderer;
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

        m_pixelizationTexture.SetData(
            nullptr, width, height);

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
} // namespace Str