#include <StarDust/Shader/ShaderProgramRegistry.h>
#include <StarDust/Utilities/Utils.h>

namespace Star
{
    ShaderProgramRegistry& ShaderProgramRegistry::Get()
    {
        static ShaderProgramRegistry instance;
        return instance;
    }

    void ShaderProgramRegistry::Register(
        const std::string& shaderPath, const std::string& shaderName)
    {
        m_registeredShaders.emplace(
            shaderName,
            ShaderProgram{
                shaderPath + "/" + shaderName + ".vert",
                shaderPath + "/" + shaderName + ".frag",
            });
    }

    ShaderProgram& ShaderProgramRegistry::GetShaderProgram(
        const std::string& name)
    {
        return m_registeredShaders.at(name);
    }

    ShaderProgramRegistry::ShaderProgramRegistry()
    {
        static const std::string ShaderPath =
            Utils::GetResourcesPath() / "Shaders";
        static const std::string ShaderNames[] = {
            "model_instance",
            "pixelate",
        };

        for (const std::string_view& shaderName : ShaderNames)
        {
            Register(ShaderPath.data(), shaderName.data());
        }
    }
} // namespace Star