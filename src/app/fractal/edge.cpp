#include "app/fractal/edge.h"
#include "app/utils/utils.h"

namespace frac {

Edge::Edge(EdgeType edgeType, unsigned int nbSubdivisions, unsigned int delay) : m_edgeType(edgeType), m_nbSubdivisions(nbSubdivisions), m_delay(delay) {}

Edge Edge::fromStr(const std::string& name) {
    std::vector<std::string> splitEdgeName = frac::utils::split(name, '_');

    EdgeType type = splitEdgeName[0] == "C" ? EdgeType::CANTOR : EdgeType::BEZIER;
    unsigned int nbSubs = std::stoul(splitEdgeName[1]);
    unsigned int delayEdge = std::stoul(splitEdgeName[2]);

    return { type, nbSubs, delayEdge };
}

void Edge::decreaseDelay() {
    if (this->isDelay()) {
        m_delay--;
    }
}

EdgeType Edge::edgeType() const {
    return m_edgeType;
}

unsigned int Edge::nbActualSubdivisions() const {
    return this->isDelay() ? 1 : m_nbSubdivisions;
}

unsigned int Edge::nbSubdivisions() const {
    return m_nbSubdivisions;
}

unsigned int& Edge::nbSubdivisions() {
    return m_nbSubdivisions;
}

unsigned int Edge::delay() const {
    return m_delay;
}

unsigned int& Edge::delay() {
    return m_delay;
}

std::vector<Edge> Edge::subdivisions(Edge const& reqEdge) const {
    std::vector<Edge> result;
    if (this->isDelay()) {
        result.emplace_back(this->edgeType(), this->nbSubdivisions(), this->delay() - 1);
    } else {
        if (this->edgeType() == EdgeType::CANTOR) {
            for (unsigned int i = 0; i < this->nbSubdivisions() - 1; ++i) {
                result.emplace_back(EdgeType::CANTOR, this->nbSubdivisions());
                result.emplace_back(reqEdge);
                result.emplace_back(reqEdge);
            }
            result.emplace_back(EdgeType::CANTOR, this->nbSubdivisions());
        }
        if (this->edgeType() == EdgeType::BEZIER) {
            for (unsigned int i = 0; i < this->nbSubdivisions(); ++i) {
                result.emplace_back(EdgeType::BEZIER, this->nbSubdivisions());
            }
        }
    }
    return result;
}

bool Edge::isDelay() const {
    return m_delay > 0;
}

namespace frac {
    std::ostream& operator<<(std::ostream& os, Edge const& edge) {
        std::string edgeType = edge.edgeType() == EdgeType::BEZIER ? "B" : "C";
        std::string nbSub = std::to_string(edge.nbSubdivisions());
        std::string delay = edge.isDelay() ? "_" + std::to_string(edge.delay()) : "";

        return os << edgeType << nbSub << delay;
    }
}

std::string Edge::name() const {
    std::string res;

    std::string edgeType = this->edgeType() == EdgeType::BEZIER ? "B" : "C";
    std::string nbSub = std::to_string(this->nbSubdivisions());
    std::string delay = this->isDelay() ? "_" + std::to_string(this->delay()) : "";

    return edgeType + nbSub + delay;
}

void Edge::setEdgeType(EdgeType edgeType) {
    m_edgeType = edgeType;
}

void Edge::setNbSubdivisions(unsigned int nbSubdivisions) {
    m_nbSubdivisions = nbSubdivisions;
}

void Edge::setDelay(unsigned int delay) {
    m_delay = delay;
}

bool Edge::operator==(Edge const& other) const {
    return m_edgeType == other.m_edgeType && m_delay == other.m_delay && m_nbSubdivisions == other.m_nbSubdivisions;
}

bool Edge::operator!=(Edge const& other) const {
    return !(*this == other);
}

std::string Edge::toString() const {
    std::string res;

    std::string edgeType = this->edgeType() == EdgeType::BEZIER ? "B_" : "C_";
    std::string nbSub = std::to_string(this->nbSubdivisions());
    std::string delay = "_" + std::to_string(this->delay());

    return edgeType + nbSub + delay;
}

std::size_t Edge::nbControlPoints(BezierType bezierType, CantorType cantorType) const {
    switch (this->edgeType()) {
        case EdgeType::CANTOR:
            switch (cantorType) {
                case CantorType::Linear_Cantor:
                    return 2;
                case CantorType::Quadratic_Cantor:
                    return 3;
                case CantorType::Cubic_Cantor:
                    return 4;
            }
            break;
        case EdgeType::BEZIER:
            switch (bezierType) {
                case BezierType::Linear_Bezier:
                    return 2;
                case BezierType::Quadratic_Bezier:
                    return 3;
                case BezierType::Cubic_Bezier:
                    return 4;
            }
    }
    //to avoid warning, but may not be executed
    return 0;
}

std::size_t Edge::nbInternControlPoints(BezierType bezierType, CantorType cantorType) const {
    return nbControlPoints(bezierType, cantorType) - 2;
}

} //frac
