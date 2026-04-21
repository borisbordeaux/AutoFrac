#ifndef AUTOFRAC_BATCHGRID_H
#define AUTOFRAC_BATCHGRID_H

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

class BatchGrid {
public:
    BatchGrid();
    void setMVP(const Core::Camera& camera, const glm::mat4& proj);
    void setBcifs(const BCIFS::Bcifs& bcifs, std::size_t gridLevel, bool displaySubdivisionPoints, bool displayControlPoints, bool displayPrimitivePoints);
    void render() const;

private:
    void addLine(const std::pair<glm::vec3, glm::vec3>& line, const glm::vec3& color);
    void addVertexLine(const glm::vec3& v, const glm::vec3& color);

private:
    Core::VertexArray m_vao;
    Core::VertexBuffer m_vbo;
    Core::VertexBufferLayout m_layout;
    Core::ShaderProgram m_program;
    std::vector<float> m_data;
    int m_count = 0;
    int m_floatsPerVertex = 6;
};

#endif //AUTOFRAC_BATCHGRID_H
