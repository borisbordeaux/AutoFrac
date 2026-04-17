#include "app/batch/batchgrid.h"

#include "app/bcifs/bcifs.h"
#include "core/camera.h"
#include "core/renderer.h"

BatchGrid::BatchGrid() {
    m_vao.bind();
    m_vbo.bind();

    m_layout.pushFloats(3); // position
    m_layout.pushFloats(3); // color

    m_vao.addBuffer(m_vbo, m_layout);

    m_program.addShaderFromFile(Core::ShaderType::Vertex, "../res/shaders/grid/vertexShader.glsl");
    m_program.addShaderFromFile(Core::ShaderType::Fragment, "../res/shaders/grid/fragmentShader.glsl");
    m_program.link();

    // unbind the vao *before* the vbo
    m_vao.unbind();
    m_vbo.unbind();
    m_program.unbind();
}

void BatchGrid::setMVP(const Core::Camera& camera, const glm::mat4& proj) {
    m_program.bind();
    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 mvp = proj * view;
    m_program.setUniformMat4f("u_mvp", mvp);
    m_program.unbind();
}

void BatchGrid::setBcifs(const BCIFS::Bcifs& bcifs, std::size_t gridLevel) {
    m_count = 0;
    m_data.clear();
    std::vector<std::pair<glm::vec3, glm::vec3>> springs = bcifs.springs(gridLevel);
    std::vector<std::pair<glm::vec3, glm::vec3>> controlPointsSprings = bcifs.controlPointsSprings(gridLevel);
    std::vector<std::pair<glm::vec3, glm::vec3>> primitiveEdges = bcifs.primitiveEdges(gridLevel);

    std::size_t nbLines = springs.size() + controlPointsSprings.size() + primitiveEdges.size();
    std::size_t nbAddsLine = 2 * nbLines;
    m_data.resize(nbAddsLine * m_floatsPerVertex);

    for (const std::pair<glm::vec3, glm::vec3>& line: controlPointsSprings) {
        this->addLine(line, glm::vec3{1.0, 0.0, 0.0});
    }
    for (const std::pair<glm::vec3, glm::vec3>& line: springs) {
        this->addLine(line, glm::vec3{0.0, 0.0, 1.0});
    }
    for (const std::pair<glm::vec3, glm::vec3>& line: primitiveEdges) {
        this->addLine(line, glm::vec3{0.0, 0.8, 0.0});
    }

    m_vbo.bind();
    m_vbo.bufferData(m_data);
    m_vbo.unbind();
}

void BatchGrid::render() const {
    m_program.bind();
    m_vao.bind();
    Core::GLCall(glDrawArrays(GL_LINES, 0, m_count / m_floatsPerVertex));
    m_program.unbind();
    m_vao.unbind();
}

void BatchGrid::addLine(const std::pair<glm::vec3, glm::vec3>& line, const glm::vec3& color) {
    this->addVertexLine(line.first, color);
    this->addVertexLine(line.second, color);
}

void BatchGrid::addVertexLine(const glm::vec3& v, const glm::vec3& color) {
    // add to the end of the data already added
    float* p = m_data.data() + m_count;
    // the coordinates of the vertex
    *p++ = v.x;
    *p++ = v.y;
    *p++ = v.z;
    *p++ = color.x;
    *p++ = color.y;
    *p = color.z;
    // we update the amount of data
    m_count += m_floatsPerVertex;
}