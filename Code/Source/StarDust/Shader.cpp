#include <StarDust/Shader.h>
#include <StarDust/Utils.h>
#include <fstream>
#include <glad/glad.h>
#include <iostream>
#include <sstream>
#include <string>

namespace Str
{
    Shader::Shader(const char* vertexPath, const char* fragmentPath)
    {
        std::string vertexSource;
        std::string fragmentSource;
        std::ifstream vertexFile, fragmentFile;
        vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try
        {
            vertexFile.open(vertexPath);
            fragmentFile.open(fragmentPath);
            std::stringstream vertexStream, fragmentStream;
            vertexStream << vertexFile.rdbuf();
            fragmentStream << fragmentFile.rdbuf();
            vertexFile.close();
            fragmentFile.close();
            vertexSource = vertexStream.str();
            fragmentSource = fragmentStream.str();
        } catch (std::ifstream::failure& e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ"
                      << std::endl;
        }

        const char* vertexShaderSource = vertexSource.c_str();
        const char* fragmentShaderSource = fragmentSource.c_str();
        unsigned int vertexShader, fragmentShader;

        vertexShader = CompileShader(vertexShaderSource, GL_VERTEX_SHADER);
        fragmentShader =
            CompileShader(fragmentShaderSource, GL_FRAGMENT_SHADER);

        GL_CHECK(m_id = glCreateProgram());
        GL_CHECK(glAttachShader(m_id, vertexShader));
        GL_CHECK(glAttachShader(m_id, fragmentShader));
        GL_CHECK(glLinkProgram(m_id));
        GL_CHECK(glValidateProgram(m_id));
        GL_CHECK(glDeleteShader(vertexShader));
        GL_CHECK(glDeleteShader(fragmentShader));
    }

    Shader::~Shader()
    {
        GL_CHECK(glDeleteProgram(m_id));
    }

    void Shader::SetUniform1i(const char* name, int value)
    {
        Bind();
        GL_CHECK(glUniform1i(GetUniformLocation(name), value));
    }

    void Shader::SetUniform1f(const char* name, float value)
    {
        Bind();
        GL_CHECK(glUniform1f(GetUniformLocation(name), value));
    }

    void Shader::SetUniform2f(const char* name, float v0, float v1)
    {
        Bind();
        GL_CHECK(glUniform2f(GetUniformLocation(name), v0, v1));
    }

    void Shader::SetUniform3f(const char* name, float v0, float v1, float v2)
    {
        Bind();
        GL_CHECK(glUniform3f(GetUniformLocation(name), v0, v1, v2));
    }

    void Shader::SetUniform4f(
        const char* name, float v0, float v1, float v2, float v3)
    {
        Bind();
        GL_CHECK(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
    }

    void Shader::SetUniformMat4f(const char* name, const glm::mat4& matrix)
    {

        Bind();
        GL_CHECK(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
    }

    void Shader::Bind() const
    {
        GL_CHECK(glUseProgram(m_id));
    }

    void Shader::Unbind()
    {
        GL_CHECK(glUseProgram(0));
    }

    unsigned int Shader::CompileShader(const char* source, unsigned int type)
    {
        unsigned int id;
        GL_CHECK(id = glCreateShader(type));
        GL_CHECK(glShaderSource(id, 1, &source, nullptr));
        GL_CHECK(glCompileShader(id));
        return id;
    }

    int Shader::GetUniformLocation(const std::string& name)
    {
        const auto& cachedLocation = m_uniformLocationCache.find(name);
        if (cachedLocation != m_uniformLocationCache.end())
        {
            return cachedLocation->second;
        }

        int location = -1;
        GL_CHECK(location = glGetUniformLocation(m_id, name.c_str()));
        m_uniformLocationCache[name] = location;
        return location;
    }
} // namespace Str