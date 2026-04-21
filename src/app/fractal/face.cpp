#include "app/fractal/face.h"
#include "app/utils/utils.h"
#include "app/fractal/algorithms/algorithmsurrounddelayandbezier.h"
#include "app/fractal/algorithms/algorithmsurrounddelay.h"
#include "app/fractal/algorithms/algorithmoncorners.h"

#include <iostream>

namespace frac {

Face::Face(std::vector<Edge> edges, unsigned int delay, Edge const& adjEdge, Edge const& gapEdge, Edge const& reqEdge, AlgorithmSubdivision algo) :
    m_data(std::move(edges)), m_delay(delay), m_adjEdge(adjEdge), m_gapEdge(gapEdge), m_reqEdge(reqEdge), m_offset(0), m_firstInterior(-1), m_algo(algo) {
    for (Face const& f : s_existingFaces.data()) {
        if (*this == f) {
            m_name = f.m_name;
            m_offset = computeOffset(f, *this);
        }
    }
    //if face doesn't exist
    if (m_name.empty()) {
        m_name = "Cell_" + std::to_string(s_existingFaces.size());
        if (m_delay != 0) {
            // one face if delayed, with subdivided boundaries
            // add delay info to name
            m_name += "_" + std::to_string(delay);
        }
        s_existingFaces.add(*this);
    }
}

Face Face::fromStr(std::string const& name) {
    std::string sepCellInfo = " / ";
    std::string sepEdges = " - ";

    std::vector<std::string> splitCellName = frac::utils::split(name, sepCellInfo);
    std::string const& edgesNames = splitCellName[0];
    std::string const& paramsNames = splitCellName[1];
    unsigned int delay = std::stoul(splitCellName[2]);

    std::vector<Edge> edges;
    for (std::string const& edgeName : frac::utils::split(edgesNames, sepEdges)) {
        edges.emplace_back(Edge::fromStr(edgeName));
    }
    std::vector<std::string> splitParamsNames = frac::utils::split(paramsNames, sepEdges);

    Edge adjEdge = Edge::fromStr(splitParamsNames[0]);
    Edge gapEdge = Edge::fromStr(splitParamsNames[1]);
    Edge reqEdge = Edge::fromStr(splitParamsNames[2]);

    AlgorithmSubdivision algo = static_cast<AlgorithmSubdivision>(std::stoul(splitCellName[3]));
    return Face(edges, delay, adjEdge, gapEdge, reqEdge, algo);
}

std::vector<Edge> const& Face::constData() const {
    return m_data;
}

std::vector<Edge>& Face::data() {
    return m_data;
}

int Face::firstInterior() const {
    return m_firstInterior;
}

int Face::lastInterior() const {
    return m_firstInterior + 2;
}

std::size_t Face::len() const {
    return m_data.size();
}

std::string Face::name() const {
    return m_name;
}

std::size_t Face::offset() const {
    return m_offset;
}

Edge Face::adjEdge() const {
    return m_adjEdge;
}

Edge Face::gapEdge() const {
    return m_gapEdge;
}

Edge Face::reqEdge() const {
    return m_reqEdge;
}

Edge& Face::adjEdge() {
    return m_adjEdge;
}

Edge& Face::gapEdge() {
    return m_gapEdge;
}

Edge& Face::reqEdge() {
    return m_reqEdge;
}

unsigned int Face::delay() const {
    return m_delay;
}

unsigned int& Face::delay() {
    return m_delay;
}

AlgorithmSubdivision Face::algo() const {
    return m_algo;
}

std::optional<Edge> Face::edgeIfRequired(const Edge& edge) const {
    if (edge.edgeType() == EdgeType::CANTOR) {
        return m_reqEdge;
    } else {
        return {};
    }
}

void Face::setAdjEdge(Edge const& edge) {
    m_adjEdge = edge;
}

void Face::setGapEdge(Edge const& edge) {
    m_gapEdge = edge;
}

void Face::setReqEdge(Edge const& edge) {
    m_reqEdge = edge;
}

void Face::setDelay(unsigned int delay) {
    m_delay = delay;
}

void Face::setFirstInterior(int index) {
    m_firstInterior = index;
}

void Face::setAlgo(AlgorithmSubdivision algo) {
    m_algo = algo;
}

std::vector<Face> Face::subdivisions() const {
    if (s_subdivisions.find(m_name) != s_subdivisions.end()) {
        return s_subdivisions[m_name];
    }
    switch (m_algo) {
        case AlgorithmSubdivision::LinksSurroundDelay:
            return LinksSurroundDelay::subdivide(*this);
        case AlgorithmSubdivision::LinksSurroundDelayAndBezier:
            return LinksSurroundDelayAndBezier::subdivide(*this);
        case AlgorithmSubdivision::LinksOnCorners:
            return LinksOnCorners::subdivide(*this);
    }
    return {};
}

Set<Face> Face::allSubdivisions() const {
    Set<Face> res;
    res.add(*this);
    std::size_t i = 0;
    bool changed = true;
    while (changed) {
        Set<Face> added;
        for (std::size_t j = i; j < res.size(); ++j) {
            std::vector<Face> subs = res[j].subdivisions();
            for (Face const& f : subs) {
                added.add(f);
            }
        }
        std::size_t lastSize = res.size();
        for (Face const& f : added.data()) {
            res.add(f);
        }
        changed = res.size() != lastSize;
        i = lastSize;
    }
    return res;
}

const Edge& Face::operator[](std::size_t index) const {
    return m_data.at(index);
}

bool Face::operator==(Face const& other) const {
    if (m_delay != other.m_delay || this->len() != other.len() || m_adjEdge != other.m_adjEdge || m_gapEdge != other.m_gapEdge || m_reqEdge != other.m_reqEdge || m_algo != other.m_algo) {
        return false;
    }
#if 0 // faces are considered only by their current boundary for comparison
    // return m_data == other.m_data;
#else // faces are considered as bracelets for comparison
    std::vector<Edge> shifted{ other.m_data };

    for (std::size_t i = 0; i < this->len(); ++i) {
        if (m_data == shifted) {
            return true;
        }
        std::vector<Edge> new_shifted{ frac::utils::shiftVector(shifted) };
        shifted.clear();
        for (Edge const& e : new_shifted) {
            shifted.emplace_back(e);
        }
    }
    return false;
#endif
}

std::ostream& operator<<(std::ostream& os, Face const& face) {
    return os << face.toString();
}

std::string Face::toString() const {
    std::string res = (*this)[0].toString();
    for (std::size_t i = 1; i < this->len(); ++i) {
        res += " - " + (*this)[i].toString();
    }
    res += " / ";

    res += m_adjEdge.toString() + " - ";
    res += m_gapEdge.toString() + " - ";
    res += m_reqEdge.toString() + " / ";
    res += std::to_string(m_delay) + " / ";
    res += std::to_string(static_cast<int>(m_algo));
    return res;
}

std::unordered_map<std::string, std::string> Face::s_incidenceConstraints;

std::unordered_map<std::string, std::string> Face::s_adjacencyConstraints;

void Face::reset() {
    Face::s_incidenceConstraints.clear();
    Face::s_adjacencyConstraints.clear();
    Face::s_existingFaces.clear();
    Face::s_subdivisions.clear();
    Face::s_incidences.clear();
}

void Face::addAdjacencyConstraint(Face const& face, Face const& faceSub1, Face const& faceSub2, unsigned int indexSubFace1, unsigned int indexBordFace1, unsigned int indexSubFace2, unsigned int indexBordFace2) {
    if (s_adjacencyConstraints.find(face.name()) == std::end(Face::s_adjacencyConstraints)) {
        s_adjacencyConstraints[face.name()] = "";
    }
    int s1 = static_cast<int>(indexSubFace1);
    int b1 = frac::utils::mod(static_cast<int>(indexBordFace1) - static_cast<int>(faceSub1.offset()), static_cast<int>(faceSub1.len()));
    int s2 = static_cast<int>(indexSubFace2);
    int b2 = frac::utils::mod(static_cast<int>(indexBordFace2) - static_cast<int>(faceSub2.offset()), static_cast<int>(faceSub2.len()));
    s_adjacencyConstraints[face.name()] += "constraint('" + face.name() + "', { 's" + std::to_string(s1) + "', 'b" + std::to_string(b1) + "', 'permut' }, { 's" + std::to_string(s2) + "', 'b" + std::to_string(b2) + "' })\n";
}

void Face::addIncidenceConstraint(Face const& face, Face const& faceSub, unsigned int indexParentEdge, unsigned int indexSubEdge, unsigned int indexSubFaceEdge, unsigned int indexSubFace) {
    if (s_incidenceConstraints.find(face.name()) == std::end(Face::s_incidenceConstraints)) {
        s_incidenceConstraints[face.name()] = "";
    }
    int b1 = frac::utils::mod(static_cast<int>(indexParentEdge) - static_cast<int>(face.offset()), static_cast<int>(face.len()));
    int s1 = static_cast<int>(indexSubEdge);
    int s2 = static_cast<int>(indexSubFace);
    int b2 = frac::utils::mod(static_cast<int>(indexSubFaceEdge) - static_cast<int>(faceSub.offset()), static_cast<int>(faceSub.len()));
    s_incidenceConstraints[face.name()] += "constraint('" + face.name() + "', { 'b" + std::to_string(b1) + "', 's" + std::to_string(s1) + "' }, { 's" + std::to_string(s2) + "', 'b" + std::to_string(b2) + "' })\n";
    s_incidences[face.name()].emplace_back(b1, s1, s2, b2);
}

std::unordered_map<std::string, std::vector<Face>> Face::s_subdivisions;

std::unordered_map<std::string, std::vector<Incidence>> Face::s_incidences;

std::size_t Face::nbControlPoints(BezierType bezierType, CantorType cantorType) const {
    std::size_t res = 0;
    for (Edge const& e : m_data) {
        res += e.nbControlPoints(bezierType, cantorType);
        res--;
    }
    return res;
}

std::vector<std::size_t> Face::controlPointIndices(std::size_t indexEdge, BezierType bezierType, CantorType cantorType, bool reverse) const {
    std::vector<std::size_t> res = {};
    std::size_t current = 0;
    for (std::size_t i = 0; i < indexEdge; i++) {
        //add to current the number of control points on the edge minus one
        current += m_data[i].nbControlPoints(bezierType, cantorType) - 1;
    }
    res.emplace_back(current); //first control point index

    std::size_t nbInternCtrlPts = m_data[indexEdge].nbInternControlPoints(bezierType, cantorType);
    for (std::size_t i = 1; i <= nbInternCtrlPts; i++) {
        res.emplace_back((current + i) % this->nbControlPoints(bezierType, cantorType)); //intern control points indices
    }

    res.emplace_back((current + nbInternCtrlPts + 1) % this->nbControlPoints(bezierType, cantorType)); //last control point index

    if (reverse) {
        std::reverse(res.begin(), res.end());
    }

    return res;
}

std::pair<std::size_t, std::size_t> Face::indexControlPointOfEdge(std::size_t indexControlPointOfFace, BezierType bezierType, CantorType cantorType) const {
    std::size_t indexControlPointOfEdge = 0;
    std::size_t indexEdge = 0;

    std::size_t currentControlPoint = 0;
    // for each edge
    for (std::size_t i = 0; i < m_data.size(); i++) {
        std::size_t nbControlPointOfEdge = m_data[i].nbControlPoints(bezierType, cantorType);
        for (std::size_t j = 0; j < nbControlPointOfEdge - 1; j++) {
            if (currentControlPoint == indexControlPointOfFace) {
                indexControlPointOfEdge = j;
                indexEdge = i;
            }
            currentControlPoint++;
        }
    }
    return { indexControlPointOfEdge, indexEdge };
}

Set<Face> Face::s_existingFaces;

std::size_t Face::computeOffset(Face const& face, Face const& other) {
    std::vector<Edge> shifted{ other.m_data };
    for (std::size_t i = 0; i < other.len(); ++i) {
        if (face.m_data == shifted) {
            return i;
        }
        shifted = frac::utils::shiftVector(shifted);
    }
    return 0;
}

} // frac
