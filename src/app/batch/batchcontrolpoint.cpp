#include "app/batch/batchcontrolpoint.h"
#include "app/bcifs/bcifs.h"
#include "core/camera.h"
#include "core/renderer.h"

BatchControlPoint::BatchControlPoint() {
    m_vao.bind();
    m_vbo.bind();

    m_layout.pushFloats(3); // position
    m_layout.pushFloats(1); // constness (const is 1.0f and var is 2.0f)

    m_vao.addBuffer(m_vbo, m_layout);

    m_program.addShaderFromFile(Core::ShaderType::Vertex, "../res/shaders/controlpoints/vertexShader.glsl");
    m_program.addShaderFromFile(Core::ShaderType::Fragment, "../res/shaders/controlpoints/fragmentShader.glsl");
    m_program.link();

    // unbind the vao *before* the vbo
    m_vao.unbind();
    m_vbo.unbind();
    m_program.unbind();
}

void BatchControlPoint::setMVP(const Core::Camera& camera, const glm::mat4& proj) {
    m_program.bind();
    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 mvp = proj * view;
    m_program.setUniformMat4f("u_mvp", mvp);
    m_program.unbind();
}

void BatchControlPoint::setBcifs(const BCIFS::Bcifs& bcifs, std::size_t gridLevel) {
    m_count = 0;
    m_data.clear();
    std::vector<BCIFS::FormalMatrix> controlPoints = bcifs.controlPoints(gridLevel);

    std::size_t nbAdds = 0;

    for (const BCIFS::FormalMatrix& matrix : controlPoints) {
        nbAdds += matrix.cols();
    }

    m_data.resize(nbAdds * m_floatsPerVertex);

    for (const BCIFS::FormalMatrix& matrix : controlPoints) {
        for (std::size_t col = 0; col < matrix.cols(); ++col) {
            bool isVar = false;
            for (std::size_t row = 0; row < matrix.rows(); ++row) {
                if (matrix.isVar(row, col)) {
                    isVar = true;
                }
            }
            this->addVertex(glm::vec3{ matrix.value(0, col), matrix.value(1, col), matrix.value(2, col) }, isVar);
        }
    }

    m_vbo.bind();
    m_vbo.bufferData(m_data);
    m_vbo.unbind();
}

void BatchControlPoint::render() const {
    m_program.bind();
    m_vao.bind();
    Core::GLCall(glDrawArrays(GL_POINTS, 0, m_count / m_floatsPerVertex));
    m_program.unbind();
    m_vao.unbind();
}

void BatchControlPoint::addVertex(const glm::vec3& v, bool isVar) {
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
