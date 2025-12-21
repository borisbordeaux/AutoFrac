#include <fstream>
#include <sstream>
#include <iostream>
#include <utility>
#include "core/shader.h"
#include "core/renderer.h"
#include "glm/glm.hpp"

namespace Core {

ShaderProgram::ShaderProgram() :
        m_vertexShader(std::nullopt), m_fragmentShader(std::nullopt), m_geometryShader(std::nullopt), m_tessellationControlShader(std::nullopt), m_tessellationEvaluationShader(std::nullopt), m_computeShader(std::nullopt) {
    GLCall(m_rendererID = glCreateProgram());
}

ShaderProgram::~ShaderProgram() {
    GLCall(glDeleteProgram(m_rendererID));
}

void ShaderProgram::addShaderFromFile(ShaderType type, std::string const& filepath) {
    std::string source = ShaderProgram::sourceOf(filepath);
    switch (type) {
        case ShaderType::Vertex:
            m_vertexShader.emplace(type, source);
            m_vertexShader->compileShader();
            break;
        case ShaderType::Fragment:
            m_fragmentShader.emplace(type, source);
            m_fragmentShader->compileShader();
            break;
        case ShaderType::Geometry:
            m_geometryShader.emplace(type, source);
            m_geometryShader->compileShader();
            break;
        case ShaderType::TessellationControl:
            m_tessellationControlShader.emplace(type, source);
            m_tessellationControlShader->compileShader();
            break;
        case ShaderType::TessellationEvaluation:
            m_tessellationEvaluationShader.emplace(type, source);
            m_tessellationEvaluationShader->compileShader();
            break;
        case ShaderType::Compute:
            m_computeShader.emplace(type, source);
            m_computeShader->compileShader();
            break;
    }
}

std::string ShaderProgram::sourceOf(std::string const& filepath) {
    std::ifstream stream(filepath);
    std::string res;
    if (stream) {
        std::stringstream ss;
        ss << stream.rdbuf();
        res = ss.str();
    }
    return res;
}

void ShaderProgram::link() {
    if (m_vertexShader.has_value()) m_vertexShader->attachShader(m_rendererID);
    if (m_fragmentShader.has_value()) m_fragmentShader->attachShader(m_rendererID);
    if (m_geometryShader.has_value()) m_geometryShader->attachShader(m_rendererID);
    if (m_tessellationControlShader.has_value()) m_tessellationControlShader->attachShader(m_rendererID);
    if (m_tessellationEvaluationShader.has_value()) m_tessellationEvaluationShader->attachShader(m_rendererID);
    if (m_computeShader.has_value()) m_computeShader->attachShader(m_rendererID);

    GLCall(glLinkProgram(m_rendererID));
    GLCall(glValidateProgram(m_rendererID));

    m_vertexShader = std::nullopt;
    m_fragmentShader = std::nullopt;
    m_geometryShader = std::nullopt;
    m_tessellationControlShader = std::nullopt;
    m_tessellationEvaluationShader = std::nullopt;
    m_computeShader = std::nullopt;
}

void ShaderProgram::bind() const {
    GLCall(glUseProgram(m_rendererID));
}

void ShaderProgram::unbind() const {
    GLCall(glUseProgram(0));
}

int ShaderProgram::getUniformLocation(std::string const& name) {
    auto it = m_uniformLocationCache.find(name);
    if (it != m_uniformLocationCache.end()) {
        return it->second;
    }
    GLCall(int location = glGetUniformLocation(m_rendererID, name.c_str()));
    if (location == -1) {
        std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;
    }
    m_uniformLocationCache.emplace(name, location);
    return location;
}

void ShaderProgram::setUniform1i(std::string const& name, int value) {
    GLCall(glUniform1i(getUniformLocation(name), value));
}

void ShaderProgram::setUniform3f(const std::string& name, const glm::vec3& v) {
    GLCall(glUniform3f(getUniformLocation(name), v.x, v.y, v.z));
}

void ShaderProgram::setUniform4f(std::string const& name, float v0, float v1, float v2, float v3) {
    GLCall(glUniform4f(getUniformLocation(name), v0, v1, v2, v3));
}

void ShaderProgram::setUniformMat4f(std::string const& name, glm::mat4 const& values) {
    GLCall(glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &values[0][0]));
}

ShaderProgram::Shader::Shader(ShaderType type, std::string src) : m_rendererID(0), m_type(type), m_source(std::move(src)) {
    GLCall(m_rendererID = glCreateShader(ShaderProgram::Shader::GLTypeOfType(type)));
}

ShaderProgram::Shader::~Shader() {
    GLCall(glDeleteShader(m_rendererID));
}

bool ShaderProgram::Shader::compileShader() {
    const char* src = m_source.c_str();
    GLCall(glShaderSource(m_rendererID, 1, &src, nullptr));
    GLCall(glCompileShader(m_rendererID));

    int result;
    GLCall(glGetShaderiv(m_rendererID, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE) {
        int length;
        GLCall(glGetShaderiv(m_rendererID, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*) alloca(length * sizeof(char));
        GLCall(glGetShaderInfoLog(m_rendererID, length, &length, message));
        std::cout << "Failed to compile ";
        std::cout << ShaderProgram::Shader::strOfType(m_type);
        std::cout << " shader!" << std::endl;
        std::cout << message << std::endl;
        GLCall(glDeleteShader(m_rendererID));
        return false;
    }

    return true;
}

std::string ShaderProgram::Shader::strOfType(ShaderType type) {
    switch (type) {
        case ShaderType::Vertex:
            return "Vertex Shader";
        case ShaderType::Fragment:
            return "Fragment Shader";
        case ShaderType::Geometry:
            return "Geometry Shader";
        case ShaderType::TessellationControl:
            return "Tessellation Control Shader";
        case ShaderType::TessellationEvaluation:
            return "Tessellation Evaluation Shader";
        case ShaderType::Compute:
            return "Compute Shader";
    }
    return "";
}

unsigned int ShaderProgram::Shader::GLTypeOfType(ShaderType type) {
    switch (type) {
        case ShaderType::Vertex:
            return GL_VERTEX_SHADER;
        case ShaderType::Fragment:
            return GL_FRAGMENT_SHADER;
        case ShaderType::Geometry:
            return GL_GEOMETRY_SHADER;
        case ShaderType::TessellationControl:
            return GL_TESS_CONTROL_SHADER;
        case ShaderType::TessellationEvaluation:
            return GL_TESS_EVALUATION_SHADER;
        case ShaderType::Compute:
            return GL_COMPUTE_SHADER;
    }
    return 0;
}

void ShaderProgram::Shader::attachShader(unsigned int programID) const {
    GLCall(glAttachShader(programID, m_rendererID));
}

} // Core