#ifndef AUTOFRAC_BATCHCONTROLPOINTSCHEME_H
#define AUTOFRAC_BATCHCONTROLPOINTSCHEME_H

#include "core/shader.h"
#include "core/vertexarray.h"
#include "core/vertexbuffer.h"
#include "core/vertexbufferlayout.h"

namespace Core {
class Camera;
}

class BatchControlPointScheme {
public:
    BatchControlPointScheme();
    void setMVP(const Core::Camera& camera, const glm::mat4& proj);
    void setControlPoints(const std::vector<std::vector<glm::vec2>>& controlPoints);
    void render() const;

private:
    void addVertex(const glm::vec2& v);

private:
    Core::VertexArray m_vao;
    Core::VertexBuffer m_vbo;
    Core::VertexBufferLayout m_layout;
    Core::ShaderProgram m_program;
    std::vector<float> m_data;
    int m_count = 0;
    int m_floatsPerVertex = 2;
};

#endif //AUTOFRAC_BATCHCONTROLPOINTSCHEME_H