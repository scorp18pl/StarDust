#pragma once

#include <StarDust/Shader/ShaderProgram.h>
#include <string>
#include <unordered_map>

namespace Star
{
    class ShaderProgramRegistry
    {
    public:
        static ShaderProgramRegistry& Get();

        //! Register a shader program.
        //! @param shaderPath The path to the directory of the shader.
        //! @param shaderName The name of the shader (file name without extension).
        void Register(const std::string& shaderPath,
            const std::string& shaderName);

        //! Get a shader program.
        ShaderProgram& GetShaderProgram(const std::string& name);
    private:
        ShaderProgramRegistry();

        std::unordered_map<std::string, ShaderProgram>
            m_registeredShaders;
    };
} // namespace Star
