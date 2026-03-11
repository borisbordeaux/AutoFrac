#include "app/batch/batchfacescheme.h"

#include "app/fractal/structure.h"
#include "app/utils/utils.h"
#include "core/application.h"
#include "core/camera.h"
#include "core/log.h"
#include "core/renderer.h"

BatchFaceScheme::BatchFaceScheme() {
    m_vao.bind();
    m_vbo.bind();

    m_layout.pushFloats(2); // position
    m_layout.pushFloats(1); // selected (positive if selected, negative otherwise)

    m_vao.addBuffer(m_vbo, m_layout);

    m_program.addShaderFromFile(Core::ShaderType::Vertex, "../res/shaders/scheme/face/vertexShader.glsl");
    m_program.addShaderFromFile(Core::ShaderType::Fragment, "../res/shaders/scheme/face/fragmentShader.glsl");
    m_program.link();

    // unbind the vao *before* the vbo
    m_vao.unbind();
    m_vbo.unbind();
    m_program.unbind();
}

void BatchFaceScheme::setMVP(const Core::Camera& camera, const glm::mat4& proj) {
    m_program.bind();
    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 mvp = proj * view;
    m_program.setUniformMat4f("u_mvp", mvp);
    m_program.unbind();
}

void BatchFaceScheme::setStructure(const frac::Structure& structure) {
    m_count = 0;
    m_data.clear();
    for (std::size_t i = 0; i < structure.faces().size(); i++) {
        this->addFace(this->tessellateFace(structure, i), m_selectedFace == i);
    }
    m_vbo.bind();
    m_vbo.bufferData(m_data);
    m_vbo.unbind();
}

void BatchFaceScheme::setSelectedFace(std::size_t index) {
    m_selectedFace = index;
}

void BatchFaceScheme::render() const {
    Core::Renderer::draw(m_vao, m_count / m_floatsPerVertex, m_program);
}

std::vector<glm::vec2> BatchFaceScheme::tessellateFace(const frac::Structure& structure, std::size_t indexFace) {
    std::vector<glm::vec2> res;
    // for each edge
    for (std::size_t i = 0; i < structure.faces()[indexFace].constData().size(); i++) {
        std::vector<std::size_t> controlPointIndices = structure.controlPointIndices(i, indexFace);
        std::size_t max = 10;
        for (std::size_t j = 0; j < max; j++) {
            float t = static_cast<float>(j) / static_cast<float>(max);
            if (controlPointIndices.size() == 2) {
                // linear
                glm::vec2 p0 = structure.controlPoints()[indexFace][controlPointIndices[0]];
                glm::vec2 p1 = structure.controlPoints()[indexFace][controlPointIndices[1]];
                res.push_back(frac::utils::coordOfPointOnLineAt(t, p0, p1));
            } else if (controlPointIndices.size() == 3) {
                // quadratic
                glm::vec2 p0 = structure.controlPoints()[indexFace][controlPointIndices[0]];
                glm::vec2 p1 = structure.controlPoints()[indexFace][controlPointIndices[1]];
                glm::vec2 p2 = structure.controlPoints()[indexFace][controlPointIndices[2]];
                res.push_back(frac::utils::coordOfPointOnQuadCurveAt(t, p0, p1, p2));
            } else {
                // cubic
                glm::vec2 p0 = structure.controlPoints()[indexFace][controlPointIndices[0]];
                glm::vec2 p1 = structure.controlPoints()[indexFace][controlPointIndices[1]];
                glm::vec2 p2 = structure.controlPoints()[indexFace][controlPointIndices[2]];
                glm::vec2 p3 = structure.controlPoints()[indexFace][controlPointIndices[3]];
                res.push_back(frac::utils::coordOfPointOnCubicCurveAt(t, p0, p1, p2, p3));
            }
        }
    }
    return res;
}

void BatchFaceScheme::addFace(const std::vector<glm::vec2>& vertices, bool selected) {
    // ear clipping method
    std::vector<std::size_t> markedVertices;

    // if the face is selected, we will
    // throw 1.0 and -1.0 otherwise
    // float isSelected = (ID == m_selectedFace && m_selectedFace != 0) ? 1.0f : -1.0f;

    std::size_t indexCurrent = 0;
    std::size_t countSinceLastMark = 0;
    while (markedVertices.size() <= vertices.size() - 3 && countSinceLastMark < vertices.size()) {
        // set indexAfter the next unmarked vertex after indexCurrent
        std::size_t indexNext = (indexCurrent + 1) % vertices.size();
        while (std::find(markedVertices.begin(), markedVertices.end(), indexNext) != markedVertices.end()) {
            indexNext = (indexNext + 1) % vertices.size();
        }

        // set indexPrev the previous unmarked vertex before indexCurrent
        std::size_t indexPrev = (indexCurrent + vertices.size() - 1) % vertices.size();
        while (std::find(markedVertices.begin(), markedVertices.end(), indexPrev) != markedVertices.end()) {
            indexPrev = (indexPrev + vertices.size() - 1) % vertices.size();
        }

        if (BatchFaceScheme::isValidTriangle(indexPrev, indexCurrent, indexNext, vertices)) {
            glm::vec2 pos0 = vertices[indexPrev];
            glm::vec2 pos1 = vertices[indexCurrent];
            glm::vec2 pos2 = vertices[indexNext];
            triangle(pos0, pos1, pos2, selected);
            markedVertices.push_back(indexCurrent);
        } else {
            countSinceLastMark++;
        }

        // set indexCurrent the next unmarked vertex
        indexCurrent = (indexCurrent + 1) % vertices.size();
        while (std::find(markedVertices.begin(), markedVertices.end(), indexCurrent) != markedVertices.end()) {
            indexCurrent = (indexCurrent + 1) % vertices.size();
        }
    }
}

void BatchFaceScheme::addVertexFace(const glm::vec2& v, float selected) {
    // add to the end of the data already added
    m_data.push_back(v.x);
    m_data.push_back(v.y);
    m_data.push_back(selected);
    // we update the amount of data
    m_count += m_floatsPerVertex;
}

bool BatchFaceScheme::isValidTriangle(std::size_t indexPrev, std::size_t indexCurrent, std::size_t indexNext, const std::vector<glm::vec2>& vertices) {
    std::vector<glm::vec2> verticesPositions;
    for (std::size_t i = 0; i < vertices.size(); i++) {
        if (i != indexPrev && i != indexCurrent && i != indexNext) {
            verticesPositions.push_back(vertices[i]);
        }
    }

    // project to plan of face to check the triangle validity
    glm::vec2 p0 = vertices[indexPrev];
    glm::vec2 p1 = vertices[indexCurrent];
    glm::vec2 p2 = vertices[indexNext];

    // triangle must be convex
    // compute angle
    glm::vec2 v10 = p0 - p1;
    glm::vec2 v12 = p2 - p1;

    float angle = std::atan2(BatchFaceScheme::cross2D(v12, v10), glm::dot(v12, v10)) * 360.0f / (2.0f * M_PIf);
    if (angle <= 0.0f || angle >= 180.0f)
        return false;

    // check no points lies into the triangle
    return std::all_of(verticesPositions.begin(), verticesPositions.end(), [&](const glm::vec2& v) { return !pointInTriangle(v, p0, p1, p2); });
}

float BatchFaceScheme::cross2D(const glm::vec2& u, const glm::vec2& v) {
    return u.x * v.y - u.y * v.x;
}

bool BatchFaceScheme::pointInTriangle(const glm::vec2& P, const glm::vec2& A, const glm::vec2& B, const glm::vec2& C) {
    glm::vec2 AB = B - A;
    glm::vec2 BC = C - B;
    glm::vec2 CA = A - C;

    glm::vec2 AP = P - A;
    glm::vec2 BP = P - B;
    glm::vec2 CP = P - C;

    float c1 = BatchFaceScheme::cross2D(AB, AP);
    float c2 = BatchFaceScheme::cross2D(BC, BP);
    float c3 = BatchFaceScheme::cross2D(CA, CP);

    bool hasNeg = (c1 < 0) || (c2 < 0) || (c3 < 0);
    bool hasPos = (c1 > 0) || (c2 > 0) || (c3 > 0);

    return !(hasNeg && hasPos);
}

void BatchFaceScheme::triangle(const glm::vec2& pos1, const glm::vec2& pos2, const glm::vec2& pos3, bool selected) {
    //add the vertices to the data
    this->addVertexFace(pos1, selected ? 1.0f : -1.0f);
    this->addVertexFace(pos2, selected ? 1.0f : -1.0f);
    this->addVertexFace(pos3, selected ? 1.0f : -1.0f);
}
