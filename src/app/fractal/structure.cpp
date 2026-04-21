#include "app/fractal/structure.h"

#include <cmath>

#include "app/utils/utils.h"
#include <iostream>

namespace frac {

Adjacency Adjacency::fromStr(std::string const& strConstraint) {
    std::string sepFaces = " / ";
    std::string sepFaceInfo = ".";

    std::vector<std::string> splitFaces = frac::utils::split(strConstraint, sepFaces);
    std::string const& face1Info = splitFaces[0];
    std::string const& face2Info = splitFaces[1];

    std::vector<std::string> splitFace1 = frac::utils::split(face1Info, sepFaceInfo);
    std::vector<std::string> splitFace2 = frac::utils::split(face2Info, sepFaceInfo);

    std::size_t face1 = std::stoul(splitFace1[0]);
    std::size_t edge1 = std::stoul(splitFace1[1]);
    std::size_t face2 = std::stoul(splitFace2[0]);
    std::size_t edge2 = std::stoul(splitFace2[1]);
    return { face1, edge1, face2, edge2 };
}

Structure::Structure(std::vector<Face> const& faces, BezierType bezierType, CantorType cantorType, ColorType colorType) :
    m_faces(faces), m_bezierType(bezierType), m_cantorType(cantorType), m_colorType(colorType) {}

void Structure::addAdjacency(Adjacency const& adj) {
    if (m_faces[adj.Face1][adj.Edge1] == m_faces[adj.Face2][adj.Edge2]) {
        std::size_t offset1 = m_faces[adj.Face1].offset();
        std::size_t offset2 = m_faces[adj.Face2].offset();
        std::size_t edge1 = static_cast<std::size_t>(frac::utils::mod(static_cast<int>(adj.Edge1) - static_cast<int>(offset1), static_cast<int>(m_faces[adj.Face1].len())));
        std::size_t edge2 = static_cast<std::size_t>(frac::utils::mod(static_cast<int>(adj.Edge2) - static_cast<int>(offset2), static_cast<int>(m_faces[adj.Face2].len())));
        m_strAdjacency += "    init(Sub('" + std::to_string(adj.Face1) + "') + Bord('" + std::to_string(edge1) + "') + Permut('0'), Sub('" + std::to_string(adj.Face2) + "') + Bord('" + std::to_string(edge2) + "'))\n";
        m_adjacencies.push_back(adj);
    }
}

std::string Structure::strAdjacencies() const {
    return this->m_strAdjacency;
}

std::vector<Adjacency> const& Structure::adjacencies() const {
    return m_adjacencies;
}

Set<Edge> Structure::allEdges() const {
    Set<Edge> res;
    Set<Face> faces = this->allFaces();
    for (Face const& f : faces.data()) {
        for (Edge const& e : f.constData()) {
            res.add(e);
        }
    }
    return res;
}

Set<Face> Structure::allFaces() const {
    Set<Face> res;
    for (Face const& f : this->m_faces) {
        res.add(f);
    }
    for (Face const& f : this->m_faces) {
        Set<Face> subdivisions = f.allSubdivisions();
        for (Face const& sub : subdivisions.data()) {
            res.add(sub);
        }
    }
    return res;
}

const std::vector<Face>& Structure::faces() const {
    return m_faces;
}

std::vector<Face>& Structure::faces() {
    return m_faces;
}

std::size_t Structure::nbControlPointsOfFace(std::size_t indexFace) const {
    return m_faces[indexFace].nbControlPoints(m_bezierType, m_cantorType);
}

std::vector<std::size_t> Structure::controlPointIndices(std::size_t indexEdge, std::size_t indexFace, bool reverse) const {
    std::vector<std::size_t> res = {};
    std::size_t current = 0;
    for (std::size_t i = 0; i < indexEdge; i++) {
        //add to current the number of control points on the edge minus one
        current += m_faces[indexFace][i].nbControlPoints(m_bezierType, m_cantorType) - 1;
    }
    res.emplace_back(current); //first control point index

    std::size_t nbInternCtrlPts = m_faces[indexFace][indexEdge].nbInternControlPoints(m_bezierType, m_cantorType);
    for (std::size_t i = 1; i <= nbInternCtrlPts; i++) {
        res.emplace_back((current + i) % this->nbControlPointsOfFace(indexFace)); //intern control points indices
    }

    res.emplace_back((current + nbInternCtrlPts + 1) % this->nbControlPointsOfFace(indexFace)); //last control point index

    if (reverse) {
        std::reverse(res.begin(), res.end());
    }

    return res;
}

bool Structure::isInternControlPoint(std::size_t indexControlPoint, std::size_t indexFace) const {
    bool res = indexControlPoint != 0;
    std::size_t current = 0;
    for (std::size_t i = 0; i < m_faces[indexFace].constData().size(); i++) {
        current += m_faces[indexFace][i].nbControlPoints(m_bezierType, m_cantorType) - 1;
        if (current == indexControlPoint) {
            res = false;
        }
    }
    return res;
}

bool Structure::isControlPointBelongEdge(std::size_t indexControlPoint, std::size_t indexFace, std::size_t indexEdge) const {
    std::vector<std::size_t> indices = this->controlPointIndices(indexEdge, indexFace);
    return std::find(indices.begin(), indices.end(), indexControlPoint) != indices.end();
}

std::ostream& operator<<(std::ostream& os, Structure const& structure) {
    for (Face const& f : structure.m_faces) {
        os << f << std::endl;
    }
    os << structure.m_strAdjacency;
    return os;
}

Face const& Structure::operator[](std::size_t index) const {
    return m_faces[index];
}

BezierType Structure::bezierType() const {
    return m_bezierType;
}

CantorType Structure::cantorType() const {
    return m_cantorType;
}

void Structure::fillControlPoints() {
    m_controlPoints.resize(m_faces.size());
    for (std::size_t indexFace = 0; indexFace < m_faces.size(); indexFace++) {
        m_controlPoints[indexFace].resize(this->nbControlPointsOfFace(indexFace));
    }

    for (std::size_t indexFace = 0; indexFace < m_faces.size(); indexFace++) {
        //barycenter coordinates
        float x = static_cast<float>(indexFace) * 3.0f;

        //number of control points that are not intern (equivalent to the number of corners)
        float nbCtrlPtsF = 0.0f;
        for (std::size_t i = 0; i < m_controlPoints[indexFace].size(); i++) {
            if (!this->isInternControlPoint(i, indexFace)) {
                nbCtrlPtsF += 1.0f;
            }
        }

        //distribute points around the origin and add the computed barycenter to place correctly the face
        double radius = 1.0f;

        //for not intern control points
        float j = 0.0f;
        for (std::size_t i = 0; i < m_controlPoints[indexFace].size(); i++) {
            if (!this->isInternControlPoint(i, indexFace)) {
                m_controlPoints[indexFace][i].x = radius * std::cos(j * 2.0f * 3.1415926f / nbCtrlPtsF) + x;
                m_controlPoints[indexFace][i].y = radius * std::sin(j * 2.0f * 3.1415926f / nbCtrlPtsF);
                j += 1.0f;
            }
        }

        //for intern control points
        int nbCtrlPts = static_cast<int>(m_controlPoints[indexFace].size());
        for (int i = 0; i < nbCtrlPts; i++) {
            if (this->isInternControlPoint(i, indexFace)) {
                std::pair<std::size_t, std::size_t> indexControlPointOfEdge = this->faces()[indexFace].indexControlPointOfEdge(i, this->bezierType(), this->cantorType());
                std::size_t indexEdge = indexControlPointOfEdge.second;
                std::size_t indexControlPoint = indexControlPointOfEdge.first;
                frac::EdgeType edgeType = this->faces()[indexFace][indexEdge].edgeType();
                bool isCubic = false;
                if (edgeType == frac::EdgeType::CANTOR) {
                    isCubic = this->cantorType() == frac::CantorType::Cubic_Cantor;
                } else {
                    isCubic = this->bezierType() == frac::BezierType::Cubic_Bezier;
                }
                if (isCubic) {
                    if (indexControlPoint == 1) {
                        glm::vec2 P0 = m_controlPoints[indexFace][i - 1];
                        glm::vec2 P1 = m_controlPoints[indexFace][(i + 2) % nbCtrlPts];
                        glm::vec2 c = frac::utils::coordOfPointOnLineAt(1.f / 3.f, P0, P1);
                        m_controlPoints[indexFace][i] = c;
                    } else {
                        glm::vec2 P0 = m_controlPoints[indexFace][i - 2];
                        glm::vec2 P1 = m_controlPoints[indexFace][(i + 1) % nbCtrlPts];
                        glm::vec2 c = frac::utils::coordOfPointOnLineAt(2.f / 3.f, P0, P1);
                        m_controlPoints[indexFace][i] = c;
                    }
                } else {
                    // quadratic
                    m_controlPoints[indexFace][i] = (m_controlPoints[indexFace][i - 1] + m_controlPoints[indexFace][(i + 1) % nbCtrlPts]) / 2.0f;
                }
            }
        }
    }
}

void Structure::setControlPoints(std::vector<std::vector<glm::vec2>> controlPoints) {
    m_controlPoints = std::move(controlPoints);
}

const std::vector<std::vector<glm::vec2>>& Structure::controlPoints() const {
    return m_controlPoints;
}

std::vector<std::vector<glm::vec2>>& Structure::controlPoints() {
    return m_controlPoints;
}

ColorType Structure::colorType() const {
    return m_colorType;
}

} // frac
