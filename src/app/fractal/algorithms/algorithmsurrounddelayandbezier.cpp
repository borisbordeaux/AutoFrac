#include "app/fractal/algorithms/algorithmsurrounddelayandbezier.h"

#include "app/fractal/face.h"
#include "app/utils/utils.h"

namespace frac::LinksSurroundDelayAndBezier {

std::vector<Face> subdivide(const Face& face) {
    std::vector<Face> res;
    bool writeConstraints = Face::s_incidenceConstraints.find(face.name()) == Face::s_incidenceConstraints.end();
    if (face.delay() == 0) {
        // if face has no delay
        std::vector<std::size_t> visitedDelayEdges;
        for (std::size_t i = 0; i < face.len(); ++i) {
            // foreach edge
            Edge current = face[i];
            if (current.isDelay()) {
                // current edge has a delay so look at edges before and after to merge also with bezier
                Edge next = face[utils::mod(i + 1, face.len())];
                Edge prev = face[static_cast<std::size_t>(utils::mod(static_cast<int>(i) - 1, static_cast<int>(face.len())))];

                if (prev.edgeType() == EdgeType::BEZIER && !prev.isDelay() && next.edgeType() == EdgeType::BEZIER && !next.isDelay()) {
                    // prev and next are bezier so merge with both of them
                    Edge subCurrent{ current };
                    subCurrent.decreaseDelay();
                    std::vector<Edge> boundaries = { prev, subCurrent, next };
                    boundaries.push_back(face.adjEdge());
                    boundaries.push_back(face.gapEdge());
                    boundaries.push_back(face.adjEdge());
                    Face c = Face(boundaries, 0, face.adjEdge(), face.gapEdge(), face.reqEdge(), face.algo());
                    c.setFirstInterior(3);
                    if (writeConstraints) {
                        Face::addIncidenceConstraint(face, c, static_cast<std::size_t>(utils::mod(static_cast<int>(i) - 1, static_cast<int>(face.len()))), prev.nbSubdivisions() - 1, 0, res.size());
                        Face::addIncidenceConstraint(face, c, i, 0, 1, res.size());
                        Face::addIncidenceConstraint(face, c, utils::mod(i + 1, face.len()), 0, 2, res.size());
                    }
                    res.push_back(c);
                    visitedDelayEdges.push_back(i);
                } else if (prev.edgeType() == EdgeType::BEZIER && !prev.isDelay()) {
                    // prev is bezier but not next so merge with prev only
                    Edge subFirst{ current };
                    subFirst.decreaseDelay();
                    std::vector<Edge> boundaries = { prev, subFirst };
                    boundaries.push_back(face.adjEdge());
                    boundaries.push_back(face.gapEdge());
                    boundaries.push_back(face.adjEdge());
                    Face c = Face(boundaries, 0, face.adjEdge(), face.gapEdge(), face.reqEdge(), face.algo());
                    c.setFirstInterior(2);
                    if (writeConstraints) {
                        Face::addIncidenceConstraint(face, c, static_cast<std::size_t>(utils::mod(static_cast<int>(i) - 1, static_cast<int>(face.len()))), prev.nbSubdivisions() - 1, 0, res.size());
                        Face::addIncidenceConstraint(face, c, i, 0, 1, res.size());
                    }
                    res.push_back(c);
                    visitedDelayEdges.push_back(i);
                } else if (next.edgeType() == EdgeType::BEZIER && !next.isDelay()) {
                    // next is bezier but not prev so merge with next only
                    Edge subFirst{ current };
                    subFirst.decreaseDelay();
                    std::vector<Edge> boundaries = { subFirst, next };
                    boundaries.push_back(face.adjEdge());
                    boundaries.push_back(face.gapEdge());
                    boundaries.push_back(face.adjEdge());
                    Face c = Face(boundaries, 0, face.adjEdge(), face.gapEdge(), face.reqEdge(), face.algo());
                    c.setFirstInterior(2);
                    if (writeConstraints) {
                        Face::addIncidenceConstraint(face, c, i, 0, 0, res.size());
                        Face::addIncidenceConstraint(face, c, frac::utils::mod(i + 1, face.len()), 0, 1, res.size());
                    }
                    res.push_back(c);
                } else if ((prev.edgeType() == EdgeType::CANTOR || prev.isDelay()) && (next.edgeType() == EdgeType::CANTOR || next.isDelay())) {
                    // next and prev are not bezier so no merge
                    Edge subFirst{ current };
                    subFirst.decreaseDelay();
                    std::vector<Edge> boundaries = { subFirst };
                    boundaries.push_back(face.adjEdge());
                    boundaries.push_back(face.gapEdge());
                    boundaries.push_back(face.adjEdge());
                    Face c = Face(boundaries, 0, face.adjEdge(), face.gapEdge(), face.reqEdge(), face.algo());
                    c.setFirstInterior(1);
                    if (writeConstraints) {
                        Face::addIncidenceConstraint(face, c, i, 0, 0, res.size());
                    }
                    res.push_back(c);
                }
            } else {
                // current edge has not a delay, so we look at the edge before if current edge is Cantor
                std::size_t idx = static_cast<std::size_t>(utils::mod(static_cast<int>(i) - 1, static_cast<int>(face.len())));
                if (face[idx].isDelay() && current.edgeType() == EdgeType::CANTOR) {
                    // the edge before has a delay and current edge is CANTOR, then we have not merged the edge so create a subcell for the edge's first subdivision
                    std::vector<Edge> boundaries = { current };
                    std::optional<Edge> requiredEdge = face.edgeIfRequired(current);
                    if (requiredEdge.has_value()) {
                        boundaries.push_back(requiredEdge.value());
                    }
                    boundaries.push_back(face.adjEdge());
                    boundaries.push_back(face.gapEdge());
                    boundaries.push_back(face.adjEdge());
                    Face c = Face(boundaries, 0, face.adjEdge(), face.gapEdge(), face.reqEdge(), face.algo());
                    c.setFirstInterior(static_cast<int>(boundaries.size() - 3));
                    if (writeConstraints) {
                        Face::addIncidenceConstraint(face, c, i, 0, 0, res.size());
                    }
                    res.push_back(c);
                }

                // creation of intermediate subcells
                int nbIntermediateStates{ static_cast<int>(current.nbSubdivisions()) - 2 };
                for (int j = 0; j < nbIntermediateStates; j++) {
                    std::vector<Edge> boundaries = { current };
                    std::optional<Edge> requiredEdge = face.edgeIfRequired(current);
                    if (requiredEdge.has_value()) {
                        boundaries.push_back(requiredEdge.value());
                    }
                    int indexFirstInterior{ static_cast<int>(boundaries.size()) };
                    boundaries.push_back(face.adjEdge());
                    boundaries.push_back(face.gapEdge());
                    boundaries.push_back(face.adjEdge());
                    if (requiredEdge.has_value()) {
                        boundaries.push_back(requiredEdge.value());
                    }
                    Face c = Face(boundaries, 0, face.adjEdge(), face.gapEdge(), face.reqEdge(), face.algo());
                    c.setFirstInterior(indexFirstInterior);
                    if (writeConstraints) {
                        Face::addIncidenceConstraint(face, c, i, j + 1, 0, res.size());
                    }
                    res.push_back(c);
                }

                // creation of last subcell of the edge, depends on the next edge
                Edge next = face[utils::mod(i + 1, face.len())];
                if (next.isDelay()) {
                    // if next edge has delay
                    if (current.edgeType() == EdgeType::CANTOR) {
                        // if current edge is cantor, create a subcell only with the last current edge subdivision
                        std::vector<Edge> boundaries = { current };
                        boundaries.push_back(face.adjEdge());
                        boundaries.push_back(face.gapEdge());
                        boundaries.push_back(face.adjEdge());
                        std::optional<Edge> requiredEdge = face.edgeIfRequired(current);
                        if (requiredEdge.has_value()) {
                            boundaries.push_back(requiredEdge.value());
                        }
                        Face c = Face(boundaries, 0, face.adjEdge(), face.gapEdge(), face.reqEdge(), face.algo());
                        c.setFirstInterior(1);
                        if (writeConstraints) {
                            Face::addIncidenceConstraint(face, c, i, nbIntermediateStates + 1, 0, res.size());
                        }
                        res.push_back(c);
                    } else if (std::find(visitedDelayEdges.begin(), visitedDelayEdges.end(), utils::mod(i + 1, face.len())) == visitedDelayEdges.end()) {
                        // if next edge that has a delay has not been visited, we need to merge with it

                        // current edge is Bezier, create subcell with subdivided delay edge
                        // and subdivided bezier edge but check also with the edge after the delay
                        // edge to merge also this one if it is bezier. If it is the case, increment i
                        // to skip the next delay edge since the subdivision is already created

                        // need to check the next's next edge
                        Edge nextNext = face[utils::mod(i + 2, face.len())];
                        if (!nextNext.isDelay() && nextNext.edgeType() == EdgeType::BEZIER) {
                            // if next's next edge is bezier without delay, merge for the subcell
                            Edge subNext{ next };
                            subNext.decreaseDelay();
                            std::vector<Edge> boundaries = { current, subNext, nextNext };
                            boundaries.push_back(face.adjEdge());
                            boundaries.push_back(face.gapEdge());
                            boundaries.push_back(face.adjEdge());
                            Face c = Face(boundaries, 0, face.adjEdge(), face.gapEdge(), face.reqEdge(), face.algo());
                            c.setFirstInterior(3);
                            if (writeConstraints) {
                                Face::addIncidenceConstraint(face, c, i, nbIntermediateStates + 1, 0, res.size());
                                Face::addIncidenceConstraint(face, c, frac::utils::mod(i + 1, face.len()), 0, 1, res.size());
                                Face::addIncidenceConstraint(face, c, frac::utils::mod(i + 2, face.len()), 0, 2, res.size());
                            }
                            res.push_back(c);
                        } else {
                            // next's next edge can't be merged (not bezier or delay)
                            Edge subNext{ next };
                            subNext.decreaseDelay();
                            std::vector<Edge> boundaries = { current, subNext };
                            boundaries.push_back(face.adjEdge());
                            boundaries.push_back(face.gapEdge());
                            boundaries.push_back(face.adjEdge());
                            Face c = Face(boundaries, 0, face.adjEdge(), face.gapEdge(), face.reqEdge(), face.algo());
                            c.setFirstInterior(2);
                            if (writeConstraints) {
                                Face::addIncidenceConstraint(face, c, i, nbIntermediateStates + 1, 0, res.size());
                                Face::addIncidenceConstraint(face, c, frac::utils::mod(i + 1, face.len()), 0, 1, res.size());
                            }
                            res.push_back(c);
                        }
                        // the next edge has been merged, so no need to create subs with it
                        i++;
                    }
                } else {
                    // if next edge has no delay, merge edges from current and next edge for the subcell
                    std::vector<Edge> boundaries = { current, next };
                    std::optional<Edge> requiredEdge = face.edgeIfRequired(next);
                    if (requiredEdge.has_value()) {
                        boundaries.push_back(requiredEdge.value());
                    }
                    int indexFirstInterior{ static_cast<int>(boundaries.size()) };
                    boundaries.push_back(face.adjEdge());
                    boundaries.push_back(face.gapEdge());
                    boundaries.push_back(face.adjEdge());
                    std::optional<Edge> secondRequiredEdge = face.edgeIfRequired(current);
                    if (secondRequiredEdge.has_value()) {
                        boundaries.push_back(secondRequiredEdge.value());
                    }
                    Face c = Face(boundaries, 0, face.adjEdge(), face.gapEdge(), face.reqEdge(), face.algo());
                    c.setFirstInterior(indexFirstInterior);
                    if (writeConstraints) {
                        Face::addIncidenceConstraint(face, c, i, nbIntermediateStates + 1, 0, res.size());
                        Face::addIncidenceConstraint(face, c, frac::utils::mod(i + 1, face.len()), 0, 1, res.size());
                    }
                    res.push_back(c);
                }
            }
        }
        for (std::size_t i = 0; i < res.size(); ++i) {
            Face const& current = res[i];
            Face const& next = res[frac::utils::mod(i + 1, res.size())];
            if (writeConstraints) {
                Face::addAdjacencyConstraint(face, current, next, i, current.firstInterior(), frac::utils::mod(i + 1, res.size()), next.lastInterior());
            }
        }
    } else {
        // current face has delay
        std::vector<Edge> boundaries = {};
        for (std::size_t i = 0; i < face.len(); ++i) {
            // for each edge, we subdivide it and add it to the result face
            Edge edge = face[i];
            std::vector<Edge> subdivisionsEdge = edge.subdivisions(face.reqEdge());
            for (Edge const& e : subdivisionsEdge) {
                boundaries.push_back(e);
            }
        }
        Face c = Face(boundaries, face.delay() - 1, face.adjEdge(), face.gapEdge(), face.reqEdge(), face.algo());
        // no adjacency constraints
        // write incidence constraints
        if (writeConstraints) {
            int k = 0;
            for (std::size_t i = 0; i < face.len(); ++i) {
                Edge edge{ face[i] };
                unsigned int nbSubdivisionsEdge = edge.nbActualSubdivisions();
                for (unsigned int j = 0; j < nbSubdivisionsEdge; ++j) {
                    Face::addIncidenceConstraint(face, c, i, j, k, 0);
                    if (edge.edgeType() == EdgeType::BEZIER) {
                        k++;
                    }
                    if (edge.edgeType() == EdgeType::CANTOR) {
                        if (edge.isDelay()) {
                            k++;
                        } else {
                            k += (j != (nbSubdivisionsEdge - 1)) ? 3 : 1;
                        }
                    }
                }
            }
        }
        res.push_back(c);
    }
    Face::s_subdivisions[face.name()] = res;
    return res;
}

} // frac::LinksSurroundDelayAndBezier
