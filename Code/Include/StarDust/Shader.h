#pragma once

#include <glm/glm.hpp>
#include <string>
#include <unordered_map>

namespace Str
{
    class Shader
    {
    public:
        Shader(const char* vertexPath, const char* fragmentPath);
        Shader(const Shader&) = delete;
        Shader(Shader&&) = delete;
        ~Shader();

        void Bind() const;
        static void Unbind();

        void SetUniform1i(const char* name, int value);
        void SetUniform1f(const char* name, float value);
        void SetUniform2f(const char* name, float v0, float v1);
        void SetUniform3f(const char* name, float v0, float v1, float v2);
        void SetUniform4f(const char* name, float v0, float v1, float v2, float v3);
        void SetUniformMat4f(const char* name, const glm::mat4& matrix);

        Shader& operator=(const Shader&) = delete;
    private:
        static unsigned int CompileShader(const char* source, unsigned int type);
        int GetUniformLocation(const std::string& name);

        std::unordered_map<std::string, int> m_uniformLocationCache;
        unsigned int m_id;
    };
} // namespace Str
