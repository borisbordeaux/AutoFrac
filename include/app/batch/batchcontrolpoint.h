#ifndef AUTOFRAC_BATCHCONTROLPOINT_H
#define AUTOFRAC_BATCHCONTROLPOINT_H

#include "core/shader.h"
#include "core/vertexarray.h"
#include "core/vertexbuffer.h"
#include "core/vertexbufferlayout.h"

namespace Core {
class Camera;
}

namespace BCIFS {
class Bcifs;
}

class BatchControlPoint {
public:
    BatchControlPoint();
    void setMVP(const Core::Camera& camera, const glm::mat4& proj);
    void setBcifs(const BCIFS::Bcifs& bcifs, std::size_t gridLevel);
    void render() const;

private:
    void addVertex(const glm::vec3& v, bool isVar);

private:
    Core::VertexArray m_vao;
    Core::VertexBuffer m_vbo;
    Core::VertexBufferLayout m_layout;
    Core::ShaderProgram m_program;
    std::vector<float> m_data;
    int m_count = 0;
    int m_floatsPerVertex = 4;
};

#endif // AUTOFRAC_BATCHCONTROLPOINT_H
