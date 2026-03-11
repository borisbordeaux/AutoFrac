#ifndef AUTOFRAC_BATCHEDGESSCHEME_H
#define AUTOFRAC_BATCHEDGESSCHEME_H

#include "core/shader.h"
#include "core/vertexarray.h"
#include "core/vertexbuffer.h"
#include "core/vertexbufferlayout.h"
#include <glm/mat4x4.hpp>

namespace Core {
class Camera;
}

namespace frac {
class Edge;
class Structure;
}

class BatchEdgesScheme {
public:
    BatchEdgesScheme();
    void setMVP(const Core::Camera& camera, const glm::mat4& proj);
    void setStructure(const frac::Structure& structure);
    void render() const;

private:
    void addLine(const std::pair<glm::vec2, glm::vec2>& line, const glm::vec3& color, float distance);
    void addVertexLine(const glm::vec2& v, const glm::vec3& color, float distance);
    void addEdge(const std::vector<glm::vec2>& vertices, const glm::vec3& color, bool isDashed);
    std::vector<glm::vec2> tessellateEdge(const frac::Structure& structure, std::size_t indexFace, std::size_t indexEdge);
    glm::vec3 colorOfEdge(const frac::Edge& edge);

private:
    Core::VertexArray m_vao;
    Core::VertexBuffer m_vbo;
    Core::VertexBufferLayout m_layout;
    Core::ShaderProgram m_program;
    std::vector<float> m_data;
    int m_count = 0;
    int m_floatsPerVertex = 6;
    glm::mat4 m_mvp;
};

#endif //AUTOFRAC_BATCHEDGESSCHEME_H