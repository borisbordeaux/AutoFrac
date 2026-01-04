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
    void setBcifs(const BCIFS::Bcifs& bcifs);
    void render() const;

private:
    void addLine(const std::pair<glm::vec3,glm::vec3>& line);
    void addVertexLine(const glm::vec3& v);

private:
    Core::VertexArray m_vaoGrid;
    Core::VertexBuffer m_vboGrid;
    Core::VertexBufferLayout m_layoutGrid;
    Core::ShaderProgram m_programGrid;
    std::vector<float> m_dataGrid;
    int m_countGrid = 0;
    int m_floatsPerVertexGrid = 3;
};

#endif //AUTOFRAC_BATCHGRID_H