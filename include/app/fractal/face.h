#ifndef AUTOFRAC_FACE_H
#define AUTOFRAC_FACE_H

#include <optional>
#include <vector>
#include <unordered_map>

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
    Face() : Face(std::vector<frac::Edge> {}) {}

    explicit Face(std::vector<frac::Edge> edges, unsigned int delay = 0, const frac::Edge& adjEdge = { frac::EdgeType::CANTOR, 2 }, const frac::Edge& gapEdge = { frac::EdgeType::BEZIER, 2 }, const frac::Edge& reqEdge = { frac::EdgeType::BEZIER, 2 }, AlgorithmSubdivision algo = AlgorithmSubdivision::LinksSurroundDelayAndBezier);
    static Face fromStr(std::string const& name);

    [[nodiscard]] std::vector<frac::Edge> const& constData() const;
    [[nodiscard]] std::vector<frac::Edge>& data();
    [[nodiscard]] int firstInterior() const;
    [[nodiscard]] int lastInterior() const;
    [[nodiscard]] std::size_t len() const;
    [[nodiscard]] std::string name() const;
    [[nodiscard]] std::size_t offset() const;
    [[nodiscard]] frac::Edge adjEdge() const;
    [[nodiscard]] frac::Edge gapEdge() const;
    [[nodiscard]] frac::Edge reqEdge() const;
    [[nodiscard]] unsigned int delay() const;
    [[nodiscard]] frac::AlgorithmSubdivision algo() const;

    [[nodiscard]] std::optional<frac::Edge> edgeIfRequired(frac::Edge const& edge) const;

    void setAdjEdge(frac::Edge const& edge);
    void setGapEdge(frac::Edge const& edge);
    void setReqEdge(frac::Edge const& edge);
    void setDelay(unsigned int delay);
    void setFirstInterior(int index);
    void setAlgo(AlgorithmSubdivision algo);

    [[nodiscard]] std::vector<frac::Face> subdivisions() const;
    [[nodiscard]] frac::Set<frac::Face> allSubdivisions() const;

    frac::Edge const& operator[](std::size_t index) const;
    bool operator==(frac::Face const& other) const;
    friend std::ostream& operator<<(std::ostream& os, const frac::Face& face);

    [[nodiscard]] std::string toString() const;

    static std::unordered_map<std::string, std::string> s_incidenceConstraints;
    static std::unordered_map<std::string, std::string> s_adjacencyConstraints;
    static void reset();

    static void addAdjacencyConstraint(frac::Face const& face, frac::Face const& faceSub1, frac::Face const& faceSub2, unsigned int indexSubFace1, unsigned int indexBordFace1, unsigned int indexSubFace2, unsigned int indexBordFace2);
    static void addIncidenceConstraint(frac::Face const& face, frac::Face const& faceSub, unsigned int indexParentEdge, unsigned int indexSubEdge, unsigned int indexSubFaceEdge, unsigned int indexSubFace);

    // key is name of the cell (since it is unique)
    static std::unordered_map<std::string, std::vector<frac::Face>> s_subdivisions;
    static std::unordered_map<std::string, std::vector<frac::Incidence>> s_incidences;

    std::size_t nbControlPoints(frac::BezierType bezierType, frac::CantorType cantorType) const;
    std::vector<std::size_t> controlPointIndices(std::size_t indexEdge, frac::BezierType bezierType, frac::CantorType cantorType, bool reverse) const;
    std::pair<std::size_t, std::size_t> indexControlPointOfEdge(std::size_t indexControlPointOfFace, frac::BezierType bezierType, frac::CantorType cantorType) const;

private:
    std::vector<frac::Edge> m_data;
    unsigned int m_delay;
    frac::Edge m_adjEdge;
    frac::Edge m_gapEdge;
    frac::Edge m_reqEdge;
    std::string m_name;
    std::size_t m_offset;
    int m_firstInterior;
    frac::AlgorithmSubdivision m_algo;

    static frac::Set<frac::Face> s_existingFaces;

    static std::size_t computeOffset(frac::Face const& face, frac::Face const& other);
};

} // frac

#endif //AUTOFRAC_FACE_H
