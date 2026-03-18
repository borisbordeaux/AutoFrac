#include "app/batch/batchface.h"

#include "app/bcifs/bcifs.h"
#include "app/bcifs/bcifsvertex.h"
#include "core/camera.h"
#include "core/renderer.h"

BatchFace::BatchFace() {
    m_vao.bind();
    m_vbo.bind();

    m_layout.pushFloats(3); // position
    m_layout.pushFloats(3); // front color
    m_layout.pushFloats(3); // back color

    m_vao.addBuffer(m_vbo, m_layout);

    m_ibo.bind();

    m_program.addShaderFromFile(Core::ShaderType::Vertex, "../res/shaders/bcifs/vertexShader.glsl");
    m_program.addShaderFromFile(Core::ShaderType::Fragment, "../res/shaders/bcifs/fragmentShader.glsl");
    m_program.link();

    // unbind the vao *before* the vbo
    m_vao.unbind();
    m_ibo.unbind();
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
    m_countIndices = 0;
    m_dataIndices.clear();
    // get all faces for the current iteration level
    // and add them to the buffer
    std::vector<std::vector<BCIFS::BcifsVertex>> faces = bcifs.faces(iterationLevel);
    m_nbFaces = faces.size();

    m_nbTriangles = 0;
    std::size_t nbVertices = 0;
    std::size_t nbIndices = 0;
    for (const std::vector<BCIFS::BcifsVertex>& face : faces) {
        std::size_t nbTriangles = face.size() > 3 ? face.size() : 1;
        m_nbTriangles += nbTriangles;
        nbVertices += nbTriangles > 3 ? face.size() + 1 : 3;
        nbIndices += nbTriangles * 3;
    }
    m_dataIndices.resize(nbIndices);
    m_data.resize(nbVertices * m_floatsPerVertex);

    for (const std::vector<BCIFS::BcifsVertex>& face : faces) {
        this->addFace(face);
    }

    m_vbo.bufferData(m_data);
    m_ibo.bufferData(m_dataIndices);

    m_data.clear();
    m_dataIndices.clear();
}

void BatchFace::render() const {
    Core::Renderer::draw(m_vao, m_ibo, m_program);
}

void BatchFace::addFace(const std::vector<BCIFS::BcifsVertex>& vertices) {
    if (vertices.size() == 3) {
        // fill indices
        unsigned int indexV0 = static_cast<unsigned int>(this->nbVertices());
        unsigned int* p = m_dataIndices.data() + m_countIndices;
        *p++ = indexV0;
        *p++ = indexV0 + 1;
        *p = indexV0 + 2;
        m_countIndices += 3;
    } else {
        glm::vec3 barycenter{ 0, 0, 0 };
        for (const BCIFS::BcifsVertex& vertex : vertices) {
            barycenter += vertex.pos();
        }
        barycenter /= static_cast<float>(vertices.size());

        // fill indices
        unsigned int indexCenter = static_cast<unsigned int>(this->nbVertices());
        unsigned int* p = m_dataIndices.data() + m_countIndices;

        for (std::size_t i = 0; i < vertices.size(); i++) {
            *p++ = indexCenter;
            *p++ = indexCenter + i + 1;
            *p++ = indexCenter + 1 + (i + 1) % vertices.size();
            m_countIndices += 3;
        }

        // fill vertices data with barycenter
        this->addVertexFace(barycenter, vertices[0].frontColor(), vertices[0].backColor());
    }
    // fill vertices data
    for (std::size_t i = 0; i < vertices.size(); i++) {
        this->addVertexFace(vertices[i].pos(), vertices[i].frontColor(), vertices[i].backColor());
    }
}

void BatchFace::addVertexFace(const glm::vec3& v, const glm::vec3& frontColor, const glm::vec3& backColor) {
    // add to the end of the data already added
    float* p = m_data.data() + m_count;
    // the coordinates of the vertex
    *p++ = v.x;
    *p++ = v.y;
    *p++ = v.z;
    // front color of the vertex
    *p++ = frontColor.x;
    *p++ = frontColor.y;
    *p++ = frontColor.z;
    // back color of the vertex
    *p++ = backColor.x;
    *p++ = backColor.y;
    *p = backColor.z;
    // we update the amount of data
    m_count += m_floatsPerVertex;
}
