#include "app/batch/batchsubdivisionpoint.h"
#include "app/bcifs/bcifs.h"
#include "core/camera.h"
#include "core/renderer.h"

BatchSubdivisionPoint::BatchSubdivisionPoint() {
    m_vao.bind();
    m_vbo.bind();

    m_layout.pushFloats(3); // position
    m_layout.pushFloats(1); // constness (const is 1.0f and var is 2.0f)

    m_vao.addBuffer(m_vbo, m_layout);

    m_program.addShaderFromFile(Core::ShaderType::Vertex, "../res/shaders/subdivisionpoints/vertexShader.glsl");
    m_program.addShaderFromFile(Core::ShaderType::Fragment, "../res/shaders/subdivisionpoints/fragmentShader.glsl");
    m_program.link();

    // unbind the vao *before* the vbo
    m_vao.unbind();
    m_vbo.unbind();
    m_program.unbind();
}

void BatchSubdivisionPoint::setMVP(const Core::Camera& camera, const glm::mat4& proj) {
    m_program.bind();
    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 mvp = proj * view;
    m_program.setUniformMat4f("u_mvp", mvp);
    m_program.unbind();
}

void BatchSubdivisionPoint::setBcifs(const BCIFS::Bcifs& bcifs) {
    m_count = 0;
    m_data.clear();

    std::pair<std::vector<BCIFS::SubdivisionPoint>, std::vector<BCIFS::SubdivisionPoint>> subdivisionPoints = bcifs.subdivisionPoints();
    std::vector<BCIFS::SubdivisionPoint> varSubdivisionPoints = subdivisionPoints.first;
    std::vector<BCIFS::SubdivisionPoint> constSubdivisionPoints = subdivisionPoints.second;

    std::size_t nbAdds = varSubdivisionPoints.size() + constSubdivisionPoints.size();

    m_data.resize(nbAdds * m_floatsPerVertex);

    for (const BCIFS::SubdivisionPoint& point : varSubdivisionPoints) {
        this->addVertex(point.posR3(), true);
    }
    for (const BCIFS::SubdivisionPoint& point : constSubdivisionPoints) {
        this->addVertex(point.posR3(), false);
    }

    m_vbo.bind();
    m_vbo.bufferData(m_data);
    m_vbo.unbind();
}

void BatchSubdivisionPoint::render() const {
    m_program.bind();
    m_vao.bind();
    Core::GLCall(glDrawArrays(GL_POINTS, 0, m_count / m_floatsPerVertex));
    m_program.unbind();
    m_vao.unbind();
}

void BatchSubdivisionPoint::addVertex(const glm::vec3& v, bool isVar) {
    // add to the end of the data already added
    float* p = m_data.data() + m_count;
    // the coordinates of the vertex
    *p++ = v.x;
    *p++ = v.y;
    *p++ = v.z;
    *p = isVar ? 2.0f : 1.0f;
    // we update the amount of data
    m_count += m_floatsPerVertex;
}
