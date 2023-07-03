#include <StarDust/Shader/ShaderProgram.h>
#include <StarDust/Utilities/Utils.h>
#include <fstream>
#include <glad/glad.h>
#include <iostream>
#include <sstream>
#include <string>

namespace Star
{
    ShaderProgram::ShaderProgram(
        const std::string& vertexPath, const std::string& fragmentPath)
        : m_vertexPath{ vertexPath }
        , m_fragmentPath{ fragmentPath }
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

    ShaderProgram::ShaderProgram(const ShaderProgram& other)
        : ShaderProgram(other.m_vertexPath, other.m_fragmentPath)
    {
    }

    ShaderProgram::~ShaderProgram()
    {
        GL_CHECK(glDeleteProgram(m_id));
    }

    void ShaderProgram::SetUniform1i(const char* name, int value)
    {
        Bind();
        GL_CHECK(glUniform1i(GetUniformLocation(name), value));
    }

    void ShaderProgram::SetUniform1f(const char* name, float value)
    {
        Bind();
        GL_CHECK(glUniform1f(GetUniformLocation(name), value));
    }

    void ShaderProgram::SetUniform2f(const char* name, float v0, float v1)
    {
        Bind();
        GL_CHECK(glUniform2f(GetUniformLocation(name), v0, v1));
    }

    void ShaderProgram::SetUniform3f(
        const char* name, float v0, float v1, float v2)
    {
        Bind();
        GL_CHECK(glUniform3f(GetUniformLocation(name), v0, v1, v2));
    }

    void ShaderProgram::SetUniform4f(
        const char* name, float v0, float v1, float v2, float v3)
    {
        Bind();
        GL_CHECK(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
    }

    void ShaderProgram::SetUniformMat4x4f(
        const char* name, const Uni::Math::Matrix4x4f& matrix)
    {
        Bind();
        GL_CHECK(glUniformMatrix4fv(
            GetUniformLocation(name),
            1,
            GL_TRUE,
            reinterpret_cast<const GLfloat*>(&matrix)));
    }

    void ShaderProgram::SetUniformMat3x4f(
        const char* name, const Uni::Math::Matrix3x4f& matrix)
    {
        Bind();
        GL_CHECK(glUniformMatrix3x4fv(
            GetUniformLocation(name),
            1,
            GL_TRUE,
            reinterpret_cast<const GLfloat*>(&matrix)));
    }

    void ShaderProgram::SetUniformMat3x4fArray(
        const char* name,
        const Uni::Math::Matrix3x4f* matrices,
        unsigned int count)
    {
        Bind();
        GL_CHECK(glUniformMatrix3x4fv(
            GetUniformLocation(name),
            count,
            GL_TRUE,
            static_cast<const GLfloat*>(static_cast<const void*>(matrices))));
    }

    void ShaderProgram::SetUniform3fArray(
        const char* name,
        const Uni::Math::Vector3f* vectors,
        unsigned int count)
    {
        Bind();
        GL_CHECK(glUniform3fv(
            GetUniformLocation(name),
            count,
            reinterpret_cast<const GLfloat*>(vectors)));
    }

    void ShaderProgram::SetUniform4fArray(
        const char* name,
        const Uni::Math::Vector4f* vectors,
        unsigned int count)
    {
        Bind();
        GL_CHECK(glUniform4fv(
            GetUniformLocation(name),
            count,
            static_cast<const GLfloat*>(static_cast<const void*>(vectors))));
    }

    void ShaderProgram::Bind() const
    {
        GL_CHECK(glUseProgram(m_id));
    }

    void ShaderProgram::Unbind()
    {
        GL_CHECK(glUseProgram(0));
    }

    unsigned int ShaderProgram::CompileShader(
        const char* source, unsigned int type)
    {
        unsigned int id;
        GL_CHECK(id = glCreateShader(type));
        GL_CHECK(glShaderSource(id, 1, &source, nullptr));
        GL_CHECK(glCompileShader(id));

        int success;
        std::string infoLog;
        GL_CHECK(glGetShaderiv(id, GL_COMPILE_STATUS, &success));
        if (!success)
        {
            GL_CHECK(glGetShaderInfoLog(id, 512, nullptr, infoLog.data()));
            throw std::runtime_error(infoLog);
        }

        return id;
    }

    int ShaderProgram::GetUniformLocation(const std::string& name)
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
} // namespace Star