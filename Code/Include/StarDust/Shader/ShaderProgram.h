#pragma once

#include <Universal/Math/Matrix/Matrix3x4f.h>
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>

namespace Str
{
    class ShaderProgram
    {
    public:
        ShaderProgram(
            const std::string& vertexPath, const std::string& fragmentPath);
        ShaderProgram(const ShaderProgram&);
        ShaderProgram(ShaderProgram&&) = delete;
        ~ShaderProgram();

        void Bind() const;
        static void Unbind();

        void SetUniform1i(const char* name, int value);
        void SetUniform1f(const char* name, float value);
        void SetUniform2f(const char* name, float v0, float v1);
        void SetUniform3f(const char* name, float v0, float v1, float v2);
        void SetUniform4f(
            const char* name, float v0, float v1, float v2, float v3);
        void SetUniformMat4f(const char* name, const glm::mat4& matrix);
        void SetUniformMat3x4f(
            const char* name, const Uni::Math::Matrix3x4f& matrix);
        void SetUniformMat3x4fArray(
            const char* name,
            const Uni::Math::Matrix3x4f* matrices,
            unsigned int count);
        void SetUniform3fArray(
            const char* name,
            const Uni::Math::Vector3f* vectors,
            unsigned int count);
        void SetUniform4fArray(
            const char* name,
            const Uni::Math::Vector4f* vectors,
            unsigned int count);

        ShaderProgram& operator=(const ShaderProgram&) = delete;

    private:
        static unsigned int CompileShader(
            const char* source, unsigned int type);
        int GetUniformLocation(const std::string& name);

        std::string m_vertexPath, m_fragmentPath;
        std::unordered_map<std::string, int> m_uniformLocationCache;
        unsigned int m_id;
    };
} // namespace Str
