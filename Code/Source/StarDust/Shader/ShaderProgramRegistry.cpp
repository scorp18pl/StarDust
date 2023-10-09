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
        const std::string& shaderPath,
        const std::string& vertexShaderName,
        const std::string& fragmentShaderName)
    {
        m_registeredShaders.emplace(
            fragmentShaderName.empty() ? vertexShaderName : fragmentShaderName,
            ShaderProgram{
                shaderPath + "/" + vertexShaderName + ".vert",
                shaderPath + "/" +
                    (fragmentShaderName.empty() ? vertexShaderName
                                                : fragmentShaderName) +
                    ".frag",
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

        Register(ShaderPath.data(), "model_instance");
        Register(ShaderPath.data(), "pixelate");

        Register(ShaderPath.data(), "model_instance", "model_instance_lighting");
    }
} // namespace Star