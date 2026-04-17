#include "app/batch/batchprimitivepoint.h"

#include "app/bcifs/bcifs.h"
#include "app/bcifs/primitivepoint.h"
#include "core/camera.h"
#include "core/renderer.h"

BatchPrimitivePoint::BatchPrimitivePoint() {
    m_vao.bind();
    m_vbo.bind();

    m_layout.pushFloats(3); // position

    m_vao.addBuffer(m_vbo, m_layout);

    m_program.addShaderFromFile(Core::ShaderType::Vertex, "../res/shaders/primitivepoints/vertexShader.glsl");
    m_program.addShaderFromFile(Core::ShaderType::Fragment, "../res/shaders/primitivepoints/fragmentShader.glsl");
    m_program.link();

    // unbind the vao *before* the vbo
    m_vao.unbind();
    m_vbo.unbind();
    m_program.unbind();
}

void BatchPrimitivePoint::setMVP(const Core::Camera& camera, const glm::mat4& proj) {
    m_program.bind();
    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 mvp = proj * view;
    m_program.setUniformMat4f("u_mvp", mvp);
    m_program.unbind();
}

void BatchPrimitivePoint::setBcifs(const BCIFS::Bcifs& bcifs, std::size_t gridLevel) {
    m_count = 0;
    m_data.clear();

    std::vector<std::vector<BCIFS::PrimitivePoint>> primitivePoints = bcifs.primitivePoints(gridLevel);

    std::size_t nbAdds = 0;

    for (const std::vector<BCIFS::PrimitivePoint>& face : primitivePoints) {
        nbAdds += face.size();
    }

    m_data.resize(nbAdds * m_floatsPerVertex);

    for (const std::vector<BCIFS::PrimitivePoint>& face : primitivePoints) {
        for (const BCIFS::PrimitivePoint& point : face) {
            this->addVertex(point.posR3());
        }
    }

    m_vbo.bind();
    m_vbo.bufferData(m_data);
    m_vbo.unbind();
}

void BatchPrimitivePoint::render() const {
    m_program.bind();
    m_vao.bind();
    Core::GLCall(glDrawArrays(GL_POINTS, 0, m_count / m_floatsPerVertex));
    m_program.unbind();
    m_vao.unbind();
}

void BatchPrimitivePoint::addVertex(const glm::vec3& v) {
    // add to the end of the data already added
    float* p = m_data.data() + m_count;
    // the coordinates of the vertex
    *p++ = v.x;
    *p++ = v.y;
    *p = v.z;
    // we update the amount of data
    m_count += m_floatsPerVertex;
}
