#include <StarDust/Shader/ShaderProgramRegistry.h>

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
        static constexpr std::string_view shaderPath =
            "/home/scorp/Documents/code/repos/StarDust/Resources/Shaders";

        static const std::string_view shaderNames[] = { "model_instance",
                                                        "pixelate" };

        for (const std::string_view& shaderName : shaderNames)
        {
            Register(shaderPath.data(), shaderName.data());
        }
    }
} // namespace Star