#ifndef AUTOFRAC_BATCHPRIMITIVEPOINT_H
#define AUTOFRAC_BATCHPRIMITIVEPOINT_H

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

class BatchPrimitivePoint {
public:
    BatchPrimitivePoint();
    void setMVP(const Core::Camera& camera, const glm::mat4& proj);
    void setBcifs(const BCIFS::Bcifs& bcifs, std::size_t gridLevel);
    void render() const;

private:
    void addVertex(const glm::vec3& v);

private:
    Core::VertexArray m_vao;
    Core::VertexBuffer m_vbo;
    Core::VertexBufferLayout m_layout;
    Core::ShaderProgram m_program;
    std::vector<float> m_data;
    int m_count = 0;
    int m_floatsPerVertex = 3;
};

#endif //AUTOFRAC_BATCHPRIMITIVEPOINT_H