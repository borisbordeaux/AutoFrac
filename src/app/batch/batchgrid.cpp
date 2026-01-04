#include "app/batch/batchgrid.h"
#include "app/bcifs/bcifs.h"
#include "core/camera.h"
#include "core/renderer.h"

BatchGrid::BatchGrid() {
    m_vaoGrid.bind();
    m_vboGrid.bind();

    m_layoutGrid.pushFloats(3); // position

    m_vaoGrid.addBuffer(m_vboGrid, m_layoutGrid);

    m_programGrid.addShaderFromFile(Core::ShaderType::Vertex, "../res/shaders/grid/vertexShader.glsl");
    m_programGrid.addShaderFromFile(Core::ShaderType::Fragment, "../res/shaders/grid/fragmentShader.glsl");
    m_programGrid.link();

    // unbind the vao *before* the vbo
    m_vaoGrid.unbind();
    m_vboGrid.unbind();
    m_programGrid.unbind();
}

void BatchGrid::setMVP(const Core::Camera& camera, const glm::mat4& proj) {
    m_programGrid.bind();
    glm::mat4 viewGrid = camera.getViewMatrix();
    glm::mat4 mvpGrid = proj * viewGrid;
    m_programGrid.setUniformMat4f("u_mvp", mvpGrid);
    m_programGrid.unbind();
}

void BatchGrid::setBcifs(const BCIFS::Bcifs& bcifs) {
    m_countGrid = 0;
    m_dataGrid.clear();
    std::vector<std::pair<glm::vec3, glm::vec3>> springs = bcifs.springs();

    std::size_t nbLines = springs.size();
    std::size_t nbAddsLine = 2 * nbLines;
    m_dataGrid.resize(nbAddsLine * m_floatsPerVertexGrid);

    for (const std::pair<glm::vec3, glm::vec3>& line: springs) {
        this->addLine(line);
    }

    m_vboGrid.bind();
    m_vboGrid.bufferData(m_dataGrid);
    m_vboGrid.unbind();
}

void BatchGrid::render() const {
    Core::GLCall(glClear(GL_DEPTH_BUFFER_BIT));
    m_programGrid.bind();
    m_vaoGrid.bind();
    Core::GLCall(glDrawArrays(GL_LINES, 0, m_countGrid / m_floatsPerVertexGrid));
    m_programGrid.unbind();
    m_vaoGrid.unbind();
}

void BatchGrid::addLine(const std::pair<glm::vec3, glm::vec3>& line) {
    this->addVertexLine(line.first);
    this->addVertexLine(line.second);
}

void BatchGrid::addVertexLine(const glm::vec3& v) {
    // add to the end of the data already added
    float* p = m_dataGrid.data() + m_countGrid;
    // the coordinates of the vertex
    *p++ = v.x;
    *p++ = v.y;
    *p++ = v.z;
    // we update the amount of data
    m_countGrid += m_floatsPerVertexGrid;
}