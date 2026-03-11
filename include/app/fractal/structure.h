#ifndef AUTOFRAC_FRAC_STRUCTURE_H
#define AUTOFRAC_FRAC_STRUCTURE_H

#include <string>
#include <vector>
#include <glm/vec2.hpp>

#include "app/fractal/face.h"
#include "app/utils/set.h"

namespace frac {

struct Adjacency {
    std::size_t Face1;
    std::size_t Edge1;
    std::size_t Face2;
    std::size_t Edge2;

    Adjacency(std::size_t face1, std::size_t edge1, std::size_t face2, std::size_t edge2) :
        Face1(face1), Edge1(edge1), Face2(face2), Edge2(edge2) {}

    bool equals(Adjacency const& other) const {
        return Face1 == other.Face1 && Edge1 == other.Edge1 && Face2 == other.Face2 && Edge2 == other.Edge2;
    }

    static Adjacency fromStr(std::string const& strConstraint);
};

class Structure {
public:
    explicit Structure(std::vector<Face> const& faces = {}, BezierType bezierType = BezierType::Quadratic_Bezier, CantorType cantorType = CantorType::Linear_Cantor, bool useColors = true);
    void addAdjacency(Adjacency const& adj);
    std::string strAdjacencies() const;
    std::vector<Adjacency> const& adjacencies() const;
    Set<Edge> allEdges() const;
    Set<Face> allFaces() const;
    std::vector<Face> const& faces() const;
    std::vector<Face>& faces();
    std::size_t nbControlPointsOfFace(std::size_t indexFace) const;
    std::vector<std::size_t> controlPointIndices(std::size_t indexEdge, std::size_t indexFace, bool reverse = false) const;
    bool isInternControlPoint(std::size_t indexControlPoint, std::size_t indexFace) const;
    bool isControlPointBelongEdge(std::size_t indexControlPoint, std::size_t indexFace, std::size_t indexEdge) const;
    friend std::ostream& operator<<(std::ostream& os, const Structure& structure);
    Face const& operator[](std::size_t index) const;
    BezierType bezierType() const;
    CantorType cantorType() const;
    void fillControlPoints();
    void setControlPoints(std::vector<std::vector<glm::vec2>> controlPoints);
    const std::vector<std::vector<glm::vec2>>& controlPoints() const;
    std::vector<std::vector<glm::vec2>>& controlPoints();
    bool useColors() const;

private:
    std::vector<Face> m_faces;
    std::string m_strAdjacency;
    std::vector<Adjacency> m_adjacencies;
    BezierType m_bezierType;
    CantorType m_cantorType;
    bool m_useColors;

    std::vector<std::vector<glm::vec2>> m_controlPoints;
};

} // frac

#endif //AUTOFRAC_FRAC_STRUCTURE_H
