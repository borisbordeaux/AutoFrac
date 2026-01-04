#ifndef AUTOFRAC_BATCHFACE_H
#define AUTOFRAC_BATCHFACE_H

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

class BatchFace {
public:
    BatchFace();
    void setMVP(const Core::Camera& camera, const glm::mat4& proj);
    void setBcifs(BCIFS::Bcifs& bcifs, int iterationLevel);

    void render() const;

    std::size_t nbFaces() const { return m_nbFaces; }
    std::size_t nbTriangles() const { return m_nbTriangles; }

private:
    void addFace(const std::vector<glm::vec3>& vertices);
    void addTriangle(const glm::vec3& pos1, const glm::vec3& pos2, const glm::vec3& pos3);
    void addVertexFace(const glm::vec3& v, const glm::vec3& n);

private:
    Core::VertexArray m_vao;
    Core::VertexBuffer m_vbo;
    Core::VertexBufferLayout m_layout;
    Core::ShaderProgram m_program;
    std::vector<float> m_data;
    int m_count = 0;
    int m_floatsPerVertex = 6;

    std::size_t m_nbFaces = 0;
    std::size_t m_nbTriangles = 0;
};

#endif //AUTOFRAC_BATCHFACE_H