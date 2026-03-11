#include "app/batch/batchedgesscheme.h"

#include <glm/ext/matrix_transform.hpp>

#include "app/fractal/structure.h"
#include "app/utils/utils.h"
#include "core/application.h"
#include "core/camera.h"
#include "core/renderer.h"

BatchEdgesScheme::BatchEdgesScheme() {
    m_vao.bind();
    m_vbo.bind();

    m_layout.pushFloats(2); // position
    m_layout.pushFloats(3); // color
    m_layout.pushFloats(1); // distance

    m_vao.addBuffer(m_vbo, m_layout);

    m_program.addShaderFromFile(Core::ShaderType::Vertex, "../res/shaders/scheme/edge/vertexShader.glsl");
    m_program.addShaderFromFile(Core::ShaderType::Geometry, "../res/shaders/scheme/edge/geometryShader.glsl");
    m_program.addShaderFromFile(Core::ShaderType::Fragment, "../res/shaders/scheme/edge/fragmentShader.glsl");
    m_program.link();

    // unbind the vao *before* the vbo
    m_vao.unbind();
    m_vbo.unbind();
    m_program.unbind();
}

void BatchEdgesScheme::setMVP(const Core::Camera& camera, const glm::mat4& proj) {
    m_program.bind();
    glm::mat4 view = camera.getViewMatrix();
    m_mvp = proj * view;
    m_program.setUniformMat4f("u_mvp", m_mvp);
    m_program.setUniform2f("u_invViewport", Core::Application::get().inverseFramebufferSize());
    m_program.unbind();
}

void BatchEdgesScheme::setStructure(const frac::Structure& structure) {
    m_count = 0;
    m_data.clear();
    for (std::size_t i = 0; i < structure.faces().size(); i++) {
        for (std::size_t j = 0; j < structure.faces()[i].constData().size(); j++) {
            bool isDashed = structure.faces()[i].constData()[j].isDelay();
            this->addEdge(this->tessellateEdge(structure, i, j), this->colorOfEdge(structure.faces()[i].constData()[j]), isDashed);
        }
    }
    m_vbo.bind();
    m_vbo.bufferData(m_data);
    m_vbo.unbind();
}

void BatchEdgesScheme::render() const {
    bool cullFaceEnabled = glIsEnabled(GL_CULL_FACE);
    if (cullFaceEnabled) {
        glDisable(GL_CULL_FACE);
    }
    m_program.bind();
    m_vao.bind();
    Core::GLCall(glDrawArrays(GL_LINES, 0, m_count / m_floatsPerVertex));
    m_program.unbind();
    m_vao.unbind();
    // restore the state
    if (cullFaceEnabled) {
        glEnable(GL_CULL_FACE);
    }
}

void BatchEdgesScheme::addLine(const std::pair<glm::vec2, glm::vec2>& line, const glm::vec3& color, float distance) {
    this->addVertexLine(line.first, color, distance);
    this->addVertexLine(line.second, color, distance);
}

void BatchEdgesScheme::addVertexLine(const glm::vec2& v, const glm::vec3& color, float distance) {
    // the coordinates of the vertex
    m_data.push_back(v.x);
    m_data.push_back(v.y);
    // the color of the vertex
    m_data.push_back(color.x);
    m_data.push_back(color.y);
    m_data.push_back(color.z);
    // the distance to the beginning of the edge
    m_data.push_back(distance);
    // we update the amount of data
    m_count += m_floatsPerVertex;
}

void BatchEdgesScheme::addEdge(const std::vector<glm::vec2>& vertices, const glm::vec3& color, bool isDashed) {
    glm::vec2 size = Core::Application::get().framebufferSize();
    glm::mat4 windowMatrix(1.0f);
    windowMatrix = glm::scale(windowMatrix, glm::vec3(size.x / 2.0f, size.y / 2.0f, 1.0f));
    windowMatrix = glm::translate(windowMatrix, glm::vec3(1.0f, 1.0f, 0.0f));

    float distance = 0.0f;
    glm::vec4 vpPt;

    for (std::size_t i = 0; i < vertices.size(); i++) {
        glm::vec4 clip = m_mvp * glm::vec4(vertices[i].x, vertices[i].y, 0.0f, 1.0f);
        glm::vec4 ndc = clip / clip.w;
        glm::vec4 vpc = windowMatrix * ndc;
        float len = i == 0 ? 0.0f : glm::length(vpPt - vpc);
        vpPt = vpc;
        if (isDashed) {
            distance += len;
        }
        this->addVertexLine(vertices[i], color, distance);
        if (i != 0 && i != vertices.size() - 1) {
            this->addVertexLine(vertices[i], color, distance);
        }
    }
}

std::vector<glm::vec2> BatchEdgesScheme::tessellateEdge(const frac::Structure& structure, std::size_t indexFace, std::size_t indexEdge) {
    std::vector<glm::vec2> res;
    std::vector<std::size_t> controlPointIndices = structure.controlPointIndices(indexEdge, indexFace);
    std::size_t max = 20;
    for (std::size_t i = 0; i < max; i++) {
        float t = static_cast<float>(i) / static_cast<float>(max);
        if (controlPointIndices.size() == 2) {
            // linear
            glm::vec2 p0 = structure.controlPoints()[indexFace][controlPointIndices[0]];
            glm::vec2 p1 = structure.controlPoints()[indexFace][controlPointIndices[1]];
            res.push_back(frac::utils::coordOfPointOnLineAt(t, p0, p1));
            if (i == max - 1) {
                res.push_back(p1);
            }
        } else if (controlPointIndices.size() == 3) {
            // quadratic
            glm::vec2 p0 = structure.controlPoints()[indexFace][controlPointIndices[0]];
            glm::vec2 p1 = structure.controlPoints()[indexFace][controlPointIndices[1]];
            glm::vec2 p2 = structure.controlPoints()[indexFace][controlPointIndices[2]];
            res.push_back(frac::utils::coordOfPointOnQuadCurveAt(t, p0, p1, p2));
            if (i == max - 1) {
                res.push_back(p2);
            }
        } else {
            // cubic
            glm::vec2 p0 = structure.controlPoints()[indexFace][controlPointIndices[0]];
            glm::vec2 p1 = structure.controlPoints()[indexFace][controlPointIndices[1]];
            glm::vec2 p2 = structure.controlPoints()[indexFace][controlPointIndices[2]];
            glm::vec2 p3 = structure.controlPoints()[indexFace][controlPointIndices[3]];
            res.push_back(frac::utils::coordOfPointOnCubicCurveAt(t, p0, p1, p2, p3));
            if (i == max - 1) {
                res.push_back(p3);
            }
        }
    }
    return res;
}

glm::vec3 BatchEdgesScheme::colorOfEdge(const frac::Edge& edge) {
    // if (edge.isDelay()) {
    //     pen.setStyle(Qt::DotLine);
    // }
    if (edge.edgeType() == frac::EdgeType::BEZIER) {
        return (glm::vec3{ 0, std::max(0, 205 - static_cast<int>(edge.nbSubdivisions() - 2) * 50), 0 }) / 255.0f;
    }
    return (glm::vec3{ 0, 0, std::max(0, 255 - static_cast<int>(edge.nbSubdivisions() - 2) * 50) }) / 255.0f;
}
