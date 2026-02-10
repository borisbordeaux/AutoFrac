#ifndef AUTOFRAC_FACE_H
#define AUTOFRAC_FACE_H

#include <optional>
#include <unordered_map>
#include <vector>

#include "app/fractal/algorithms/algorithmsubdivision.h"
#include "app/fractal/edge.h"
#include "app/utils/set.h"

namespace frac {

struct Incidence {
    std::size_t Edge1;
    std::size_t SubEdge1;
    std::size_t SubFace2;
    std::size_t Edge2;

    Incidence(std::size_t edge1, std::size_t subEdge1, std::size_t subFace2, std::size_t edge2) :
        Edge1(edge1), SubEdge1(subEdge1), SubFace2(subFace2), Edge2(edge2) {}
};

class Face {
public:
    Face() : Face(std::vector<Edge>{}) {}
    explicit Face(std::vector<Edge> edges, unsigned int delay = 0, const Edge& adjEdge = { EdgeType::CANTOR, 2 }, const Edge& gapEdge = { EdgeType::BEZIER, 2 }, const Edge& reqEdge = { EdgeType::BEZIER, 2 }, AlgorithmSubdivision algo = AlgorithmSubdivision::LinksSurroundDelayAndBezier);
    static Face fromStr(std::string const& name);
    std::vector<Edge> const& constData() const;
    std::vector<Edge>& data();
    int firstInterior() const;
    int lastInterior() const;
    std::size_t len() const;
    std::string name() const;
    std::size_t offset() const;
    Edge adjEdge() const;
    Edge gapEdge() const;
    Edge reqEdge() const;
    Edge& adjEdge();
    Edge& gapEdge();
    Edge& reqEdge();
    unsigned int delay() const;
    unsigned int& delay();
    AlgorithmSubdivision algo() const;
    std::optional<Edge> edgeIfRequired(Edge const& edge) const;
    void setAdjEdge(Edge const& edge);
    void setGapEdge(Edge const& edge);
    void setReqEdge(Edge const& edge);
    void setDelay(unsigned int delay);
    void setFirstInterior(int index);
    void setAlgo(AlgorithmSubdivision algo);
    std::vector<Face> subdivisions() const;
    Set<Face> allSubdivisions() const;
    Edge const& operator[](std::size_t index) const;
    bool operator==(Face const& other) const;
    friend std::ostream& operator<<(std::ostream& os, const Face& face);
    std::string toString() const;
    static std::unordered_map<std::string, std::string> s_incidenceConstraints;
    static std::unordered_map<std::string, std::string> s_adjacencyConstraints;
    static void reset();
    static void addAdjacencyConstraint(Face const& face, Face const& faceSub1, Face const& faceSub2, unsigned int indexSubFace1, unsigned int indexBordFace1, unsigned int indexSubFace2, unsigned int indexBordFace2);
    static void addIncidenceConstraint(Face const& face, Face const& faceSub, unsigned int indexParentEdge, unsigned int indexSubEdge, unsigned int indexSubFaceEdge, unsigned int indexSubFace);
    // key is name of the cell (since it is unique)
    static std::unordered_map<std::string, std::vector<Face>> s_subdivisions;
    static std::unordered_map<std::string, std::vector<Incidence>> s_incidences;
    std::size_t nbControlPoints(BezierType bezierType, CantorType cantorType) const;
    std::vector<std::size_t> controlPointIndices(std::size_t indexEdge, BezierType bezierType, CantorType cantorType, bool reverse) const;
    std::pair<std::size_t, std::size_t> indexControlPointOfEdge(std::size_t indexControlPointOfFace, BezierType bezierType, CantorType cantorType) const;

private:
    std::vector<Edge> m_data;
    unsigned int m_delay;
    Edge m_adjEdge;
    Edge m_gapEdge;
    Edge m_reqEdge;
    std::string m_name;
    std::size_t m_offset;
    int m_firstInterior;
    AlgorithmSubdivision m_algo;
    static Set<Face> s_existingFaces;
    static std::size_t computeOffset(Face const& face, Face const& other);
};

} // frac

#endif //AUTOFRAC_FACE_H
