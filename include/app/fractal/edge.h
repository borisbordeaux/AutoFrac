#ifndef EDGE_H
#define EDGE_H

#include <ostream>
#include <string>
#include <vector>

namespace frac {

enum class EdgeType {
    CANTOR, BEZIER
};

enum class CantorType {
    Linear_Cantor,
    Quadratic_Cantor,
    Cubic_Cantor
};

enum class BezierType {
    Linear_Bezier,
    Quadratic_Bezier,
    Cubic_Bezier
};

class Edge {
public:
    Edge(Edge const& other) = default;
    Edge(EdgeType edgeType, unsigned int nbSubdivisions, unsigned int delay = 0);
    Edge& operator=(const Edge& other) = default;
    static Edge fromStr(std::string const& name);
    void decreaseDelay();
    EdgeType edgeType() const;
    unsigned int nbSubdivisions() const;
    unsigned int nbActualSubdivisions() const;
    unsigned int delay() const;
    std::vector<Edge> subdivisions(Edge const& reqEdge) const;
    bool isDelay() const;
    std::string name() const;
    void setEdgeType(EdgeType edgeType);
    void setNbSubdivisions(unsigned int nbSubdivisions);
    void setDelay(unsigned int delay);
    friend std::ostream& operator<<(std::ostream& os, Edge const& edge);
    bool operator==(Edge const& other) const;
    bool operator!=(Edge const& other) const;
    std::string toString() const;
    std::size_t nbControlPoints(BezierType bezierType, CantorType cantorType) const;
    std::size_t nbInternControlPoints(BezierType bezierType, CantorType cantorType) const;

private:
    EdgeType m_edgeType;
    unsigned int m_nbSubdivisions;
    unsigned int m_delay;
};

} // frac

#endif // EDGE_H


