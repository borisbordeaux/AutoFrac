#include "app/batch/batchcontrolpointscheme.h"

#include "core/camera.h"
#include "core/renderer.h"

BatchControlPointScheme::BatchControlPointScheme() {
    m_vao.bind();
    m_vbo.bind();

    m_layout.pushFloats(2); // position

    m_vao.addBuffer(m_vbo, m_layout);

    m_program.addShaderFromFile(Core::ShaderType::Vertex, "../res/shaders/scheme/vertex/vertexShader.glsl");
    m_program.addShaderFromFile(Core::ShaderType::Fragment, "../res/shaders/scheme/vertex/fragmentShader.glsl");
    m_program.link();

    // unbind the vao *before* the vbo
    m_vao.unbind();
    m_vbo.unbind();
    m_program.unbind();
}

void BatchControlPointScheme::setMVP(const Core::Camera& camera, const glm::mat4& proj) {
    m_program.bind();
    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 mvp = proj * view;
    m_program.setUniformMat4f("u_mvp", mvp);
    m_program.unbind();
}

void BatchControlPointScheme::setControlPoints(const std::vector<std::vector<glm::vec2>>& controlPoints) {
    m_count = 0;
    m_data.clear();

    std::size_t nbAdds = 0;

    for (const std::vector<glm::vec2>& currentFaceControlPoints : controlPoints) {
        nbAdds += currentFaceControlPoints.size();
    }

    m_data.resize(nbAdds * m_floatsPerVertex);

    for (const std::vector<glm::vec2>& currentFaceControlPoints : controlPoints) {
        for (const glm::vec2& controlPoint : currentFaceControlPoints) {
            this->addVertex(controlPoint);
        }
    }

    m_vbo.bind();
    m_vbo.bufferData(m_data);
    m_vbo.unbind();
}

void BatchControlPointScheme::render() const {
    m_program.bind();
    m_vao.bind();
    Core::GLCall(glDrawArrays(GL_POINTS, 0, m_count / m_floatsPerVertex));
    m_program.unbind();
    m_vao.unbind();
}

void BatchControlPointScheme::addVertex(const glm::vec2& v) {
    // add to the end of the data already added
    float* p = m_data.data() + m_count;
    // the coordinates of the vertex
    *p++ = v.x;
    *p = v.y;
    // we update the amount of data
    m_count += m_floatsPerVertex;
}
