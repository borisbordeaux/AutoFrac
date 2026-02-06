#ifndef AUTOFRAC_SHADER_H
#define AUTOFRAC_SHADER_H

#include <glm/fwd.hpp>
#include <optional>
#include <string>
#include <unordered_map>

namespace Core {

enum class ShaderType {
    Vertex, Fragment, Geometry, TessellationControl, TessellationEvaluation, Compute
};

class ShaderProgram {
public:
    ShaderProgram();
    ~ShaderProgram();
    void addShaderFromFile(ShaderType type, std::string const& filepath);
    void link();
    void bind() const;
    void unbind() const;
    void setUniform1i(std::string const& name, int value);
    void setUniform3f(std::string const& name, const glm::vec3& v);
    void setUniform4f(std::string const& name, float v0, float v1, float v2, float v3);
    void setUniformMat4f(std::string const& name, const glm::mat4& values);

private:
    static std::string sourceOf(std::string const& filepath);
    int getUniformLocation(std::string const& name);

private:
    class Shader {
    public:
        Shader(ShaderType type, std::string src);
        ~Shader();
        bool compileShader();
        void attachShader(unsigned int programID) const;

    private:
        static std::string strOfType(ShaderType type);
        static unsigned int GLTypeOfType(ShaderType type);

    private:
        unsigned int m_rendererID;
        ShaderType m_type;
        const std::string m_source;
    };

private:
    unsigned int m_rendererID;
    std::optional<Shader> m_vertexShader;
    std::optional<Shader> m_fragmentShader;
    std::optional<Shader> m_geometryShader;
    std::optional<Shader> m_tessellationControlShader;
    std::optional<Shader> m_tessellationEvaluationShader;
    std::optional<Shader> m_computeShader;
    std::unordered_map<std::string, int> m_uniformLocationCache;
};

} // Core

#endif //AUTOFRAC_SHADER_H
