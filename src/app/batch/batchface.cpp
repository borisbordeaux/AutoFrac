#include "app/batch/batchface.h"
#include "app/bcifs/bcifs.h"
#include "core/camera.h"
#include "core/renderer.h"

BatchFace::BatchFace() {
    m_vao.bind();
    m_vbo.bind();

    m_layout.pushFloats(3); // position
    m_layout.pushFloats(3); // normal
    m_layout.pushFloats(3); // front color
    m_layout.pushFloats(3); // back color

    m_vao.addBuffer(m_vbo, m_layout);

    m_program.addShaderFromFile(Core::ShaderType::Vertex, "../res/shaders/bcifs/vertexShader.glsl");
    m_program.addShaderFromFile(Core::ShaderType::Fragment, "../res/shaders/bcifs/fragmentShader.glsl");
    m_program.link();

    // unbind the vao *before* the vbo
    m_vao.unbind();
    m_vbo.unbind();
    m_program.unbind();
}

void BatchFace::setMVP(const Core::Camera& camera, const glm::mat4& proj) {
    m_program.bind();
    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 mvp = proj * view;
    m_program.setUniformMat4f("u_mvp", mvp);
    m_program.setUniform3f("lightPos", camera.getEye());
    m_program.setUniform3f("cameraPos", camera.getEye());
    m_program.unbind();
}

void BatchFace::setIlluminationMode(IlluminationMode mode) {
    m_program.bind();
    m_program.setUniform1i("illuminationMode", static_cast<int>(mode));
    m_program.unbind();
}

void BatchFace::setBcifs(BCIFS::Bcifs& bcifs, int iterationLevel) {
    m_count = 0;
    m_data.clear();
    // get all faces for the current iteration level
    // and add them to the buffer
    std::vector<std::vector<BCIFS::BcifsPoint>> faces = bcifs.faces(iterationLevel);
    m_nbFaces = faces.size();

    m_nbTriangles = 0;
    for (const std::vector<BCIFS::BcifsPoint>& face : faces) {
        m_nbTriangles += face.size();
    }
    std::size_t nbAdds = 3 * m_nbTriangles;
    m_data.resize(nbAdds * m_floatsPerVertex);

    for (const std::vector<BCIFS::BcifsPoint>& face : faces) {
        this->addFace(face);
    }

    m_vbo.bind();
    m_vbo.bufferData(m_data);
    m_vbo.unbind();
}

void BatchFace::render() const {
    Core::Renderer::draw(m_vao, m_count / m_floatsPerVertex, m_program);
}

void BatchFace::addFace(const std::vector<BCIFS::BcifsPoint>& vertices) {
    glm::vec3 barycenter{ 0, 0, 0 };
    for (const BCIFS::BcifsPoint& vertex : vertices) {
        barycenter += vertex.pos();
    }
    barycenter /= static_cast<float>(vertices.size());
    for (std::size_t i = 0; i < vertices.size(); i++) {
        this->addTriangle(barycenter, vertices[i].pos(), vertices[(i + 1) % vertices.size()].pos(), vertices[0].frontColor(), vertices[0].backColor());
    }
}

void BatchFace::addTriangle(const glm::vec3& pos1, const glm::vec3& pos2, const glm::vec3& pos3, const glm::vec3& frontColor, const glm::vec3& backColor) {
    //compute the normal of the triangleSphere
    glm::vec3 n = glm::normalize(glm::cross(pos2 - pos1, pos3 - pos2));

    //add the vertices to the data
    this->addVertexFace(pos1, n, frontColor, backColor);
    this->addVertexFace(pos2, n, frontColor, backColor);
    this->addVertexFace(pos3, n, frontColor, backColor);
}

void BatchFace::addVertexFace(const glm::vec3& v, const glm::vec3& n, const glm::vec3& frontColor, const glm::vec3& backColor) {
    // add to the end of the data already added
    float* p = m_data.data() + m_count;
    // the coordinates of the vertex
    *p++ = v.x;
    *p++ = v.y;
    *p++ = v.z;
    // the normal of the vertex
    *p++ = n.x;
    *p++ = n.y;
    *p++ = n.z;
    // front color of the vertex
    *p++ = frontColor.x;
    *p++ = frontColor.y;
    *p++ = frontColor.z;
    // back color of the vertex
    *p++ = backColor.x;
    *p++ = backColor.y;
    *p++ = backColor.z;
    // we update the amount of data
    m_count += m_floatsPerVertex;
}
