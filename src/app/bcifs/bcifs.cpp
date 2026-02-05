#include "app/bcifs/bcifs.h"

#include "app/bcifs/state.h"
#include "app/bcifs/booleanmatrix.h"
#include "app/bcifs/constraintsolver.h"
#include "app/bcifs/subdivisionpoint.h"
#include "app/bcifs/utils.h"
#include "app/bcifs/bcifsvertex.h"
#include "core/log.h"

#include <algorithm>

namespace BCIFS {

std::pair<StateID, std::vector<TransitionID>> Bcifs::addState(std::string name, std::size_t internalDimensions) {
    StateID id = m_automaton.addState(std::move(name));
    std::vector<TransitionID> internalTransitions(internalDimensions);
    for (std::size_t i = 0; i < internalDimensions; i++) {
        internalTransitions[i] = this->addInternal(std::to_string(i), id);
    }
    return { id, std::move(internalTransitions) };
}

StateID Bcifs::addInitState() {
    auto [id, _] = this->addState("#", 3);
    m_initStateID.emplace(id);
    return id;
}

TransitionID Bcifs::addBoundary(std::string name, StateID from, StateID to) {
    return this->addTransition(std::move(name), from, to, TransitionType::BOUNDARY);
}

void Bcifs::addGrid(StateID id, Grid grid) {
    m_mapGrids.emplace(id, std::move(grid));
}

void Bcifs::addGridFromBoundary(StateID id) {
    m_createGridFromBoundary.push_back(id);
}

void Bcifs::setSpace(StateID id, std::vector<TransitionID> transitions) {
    m_mapSpaces[id] = std::move(transitions);
}

void Bcifs::setPrimitive(StateID id, std::vector<Figure> primitive) {
    m_mapPrimitives[id] = std::move(primitive);
}

TransitionID Bcifs::addSubdivision(std::string name, StateID from, StateID to) {
    return this->addTransition(std::move(name), from, to, TransitionType::SUBDIVISION);
}

TransitionID Bcifs::addSubdivision(std::string name, StateID from, StateID to, glm::vec3 frontColor) {
    TransitionID transitionId = this->addTransition(std::move(name), from, to, TransitionType::SUBDIVISION);
    m_frontColors[transitionId] = std::move(frontColor);
    return transitionId;
}

TransitionID Bcifs::addSubdivision(std::string name, StateID from, StateID to, glm::vec3 frontColor, glm::vec3 backColor) {
    TransitionID transitionId = this->addTransition(std::move(name), from, to, TransitionType::SUBDIVISION);
    m_frontColors[transitionId] = std::move(frontColor);
    m_backColors[transitionId] = std::move(backColor);
    return transitionId;
}

TransitionID Bcifs::addPermutation(std::string name, StateID from, StateID to) {
    return this->addTransition(std::move(name), from, to, TransitionType::PERMUTATION);
}

void Bcifs::addConstraint(const Path& lhs, const Path& rhs) {
    if (lhs.empty() || rhs.empty()) throw std::runtime_error("Constraint must not be empty");

    Constraint constraint(lhs, rhs);
    switch (this->constraintType(constraint)) {
        case ConstraintType::SUBDIVISION:
            m_constraints.push_back(std::move(constraint));
            break;
        case ConstraintType::ADJACENCY_ON_INCIDENCE_OPERATORS:
            m_adjacencyConstraintsOnIncidenceOperators.push_back(std::move(constraint));
            break;
        case ConstraintType::PERMUTATION:
            m_permutationConstraints.push_back(std::move(constraint));
            break;
    }
}

void Bcifs::setInitMat(TransitionID id, const FormalMatrix& matrix) {
    m_mapInitMat.emplace(id, matrix);
}

std::string Bcifs::toString() const {
    std::string res = "BCIFS Description:\n";
    res += m_automaton.toString();
    res += "Classic constraints:\n";
    for (const auto& constraint : m_constraints) {
        res += this->printConstraint(constraint);
    }
    res += "\nAdjacency constraints on incidence operators:\n";
    for (const auto& constraint : m_adjacencyConstraintsOnIncidenceOperators) {
        res += this->printConstraint(constraint);
    }
    res += "\nDimensions of states:\n";
    for (const std::pair<const StateID, std::size_t>& keyval : m_mapDimensions) {
        res += m_automaton.findStateByID(keyval.first).name() + " has " + std::to_string(keyval.second) + " dimensions\n";
    }
    res += "\nAll operators:\n";
    for (const std::pair<const TransitionID, FormalMatrix>& keyval : m_mapOperators) {
        TransitionID transitionId = keyval.first;
        const Transition& transition = m_automaton.findTransitionByID(transitionId);
        res += "Operator ";
        res += transition.toString();
        res += " from " + m_automaton.findStateByID(transition.from()).name();
        res += " to " + m_automaton.findStateByID(transition.to()).name() + "\n";
        if (transition.type() == TransitionType::SUBDIVISION) {
            res += keyval.second.toString(true);
            res += "-------\n";
        }
        res += keyval.second.toString();
    }
    res += "\nAll grids:\n";
    for (const std::pair<const StateID, Grid>& keyval : m_mapGrids) {
        res += "For state " + std::to_string(keyval.first) + ", grid is:\n";
        for (const GridFigure& gridFigure : keyval.second) {
            res += "Figure [";
            for (const Path& path : gridFigure.paths()) {
                res += " [";
                for (const TransitionID& transitionId : path) {
                    res += " ";
                    const Transition& transition = m_automaton.findTransitionByID(transitionId);
                    res += transition.toString();
                }
                res += " ] ";
            }
            res += ", k=" + utils::toString(gridFigure.k()) + ", length=" + utils::toString(gridFigure.length()) + "]\n";
        }
        res += "\n";
    }
    return res;
}

void Bcifs::print() const {
    Core::LOG_INFO(this->toString());
}

void Bcifs::check() const {
    this->checkAutomaton();   // all states used in transitions exist
    this->checkSpaces();      // all states have a valid space
    this->checkConstraints(); // same arrival (existing) state for each path
}

void Bcifs::finalize() {
    this->initializeMatrices();          // initialize all boundary and internal operators
    this->resolveConstraints();          // resolve all constraints to finish the matrices initialization
    this->initSubdivisionOperators();    // initialize all subdivision operators not implied in a constraint
    this->completeSubdivisionMatrices(); // make sure matrices are barycentric transformations
    this->buildGridsFromBoundary();
    this->buildMassSpringSystems();   // initialize all mass spring systems for each state with a user defined grid
    this->buildMSSForControlPoints(); // initialize all mass spring systems for the control points
}

void Bcifs::reset() {
    m_automaton.reset();
    m_initStateID = std::nullopt;
    m_mapSpaces.clear();
    m_constraints.clear();
    m_adjacencyConstraintsOnIncidenceOperators.clear();
    m_permutationConstraints.clear();
    m_mapDimensions.clear();
    m_mapOperators.clear();
    m_mapGrids.clear();
    m_createGridFromBoundary.clear();
    m_mapMSS.clear();
    m_MSSControlPoints.clear();
    m_mapInitMat.clear();
    m_invalidatedMatrices = true;
    m_invalidatedMatricesControlPoints = false;
    m_facesPaths.clear();
    m_mapPrimitives.clear();
    m_mapPrimitivesMat.clear();
    m_needUpdatePrimitivesWhenChangingMatrices = true;
    m_frontColors.clear();
    m_backColors.clear();
    m_colorDepth = 0;
    m_pool.reset();
}

std::vector<std::vector<BcifsVertex>> Bcifs::faces(int iterationLevel) {
    if (!m_initStateID.has_value()) { return {}; }
    if (m_invalidatedMatrices) {
        m_invalidatedMatrices = false;

        m_mapOperatorsMat.clear();
        for (const std::pair<const TransitionID, FormalMatrix>& keyval : m_mapOperators) {
            m_mapOperatorsMat.insert({ keyval.first, keyval.second.toMat() });
        }

        m_facesPaths.clear();
        if (iterationLevel != 0) {
            // each path will give a matrix that contains the position of all the points of a face
            std::vector<Path> allPaths = m_automaton.allSubdivisionPaths(m_initStateID.value(), iterationLevel + 1);

            for (const Path& path : allPaths) {
                arma::mat mat = this->getOperatorOfPath(path);
                m_facesPaths.emplace_back(path, mat);
            }
        } else {
            for (TransitionID transitionId : m_automaton.subdivisionTransitionsOf(m_initStateID.value())) {
                m_facesPaths.emplace_back(Path({ transitionId }), this->getOperatorMat(transitionId));
            }
        }

        if (m_needUpdatePrimitivesWhenChangingMatrices) {
            // if primitives don't depend on subdivision matrices,
            // they will be initialized only one time
            m_needUpdatePrimitivesWhenChangingMatrices = false;
            this->initPrimitives();
        }
    } else if (m_invalidatedMatricesControlPoints) {
        m_invalidatedMatricesControlPoints = false;
        for (TransitionID transitionId : m_automaton.subdivisionTransitionsOf(m_initStateID.value())) {
            m_mapOperatorsMat.at(transitionId) = m_mapOperators.at(transitionId).toMat();
        }
        if (iterationLevel == 0) {
            m_facesPaths.clear();
            for (TransitionID transitionId : m_automaton.subdivisionTransitionsOf(m_initStateID.value())) {
                m_facesPaths.emplace_back(Path({ transitionId }), this->getOperatorMat(transitionId));
            }
        }
    }

    std::vector<std::vector<BcifsVertex>> res;
    for (const std::pair<Path, arma::mat>& keyval : m_facesPaths) {
        if (iterationLevel != 0) {
            TransitionID lastTransitionId = keyval.first[keyval.first.size() - 1];
            const Transition& lastTransition = m_automaton.findTransitionByID(lastTransitionId);
            const std::vector<arma::mat>& primitiveMatrices = this->getPrimitiveMat(lastTransition.to());
            glm::vec3 frontColor = this->getFrontColor(keyval.first);
            glm::vec3 backColor = this->getBackColor(keyval.first);
            // each primitive mat is a face
            for (const arma::mat& primitive : primitiveMatrices) {
                std::vector<BcifsVertex> vertices;
                arma::mat mat = this->getOperatorMat(keyval.first[0]) * keyval.second * primitive;
                vertices.reserve(mat.n_cols);
                for (std::size_t i = 0; i < mat.n_cols; i++) {
                    glm::vec3 pos(mat.at(0, i), mat.at(1, i), mat.at(2, i));
                    vertices.emplace_back(pos, frontColor, backColor);
                }
                res.push_back(vertices);
            }
        } else {
            TransitionID lastTransitionId = keyval.first[keyval.first.size() - 1];
            const Transition& lastTransition = m_automaton.findTransitionByID(lastTransitionId);
            const std::vector<arma::mat>& primitiveMatrices = this->getPrimitiveMat(lastTransition.to());
            glm::vec3 frontColor = this->getFrontColor(keyval.first);
            glm::vec3 backColor = this->getBackColor(keyval.first);
            // each primitive mat is a face
            for (const arma::mat& primitive : primitiveMatrices) {
                std::vector<BcifsVertex> vertices;
                arma::mat mat = keyval.second * primitive;
                vertices.reserve(mat.n_cols);
                for (std::size_t i = 0; i < mat.n_cols; i++) {
                    glm::vec3 pos(mat.at(0, i), mat.at(1, i), mat.at(2, i));
                    vertices.emplace_back(pos, frontColor, backColor);
                }
                res.push_back(vertices);
            }
        }
    }

    if (!m_cacheTransforms) {
        m_mapOperatorsMat.clear();
        m_facesPaths.clear();
        m_invalidatedMatrices = true;
    }
    return res;
}

std::vector<FormalMatrix> Bcifs::controlPoints(std::size_t gridLevel) const {
    if (!m_initStateID.has_value() || gridLevel > 1) { return {}; }
    std::vector<FormalMatrix> res;
    std::vector<TransitionID> transitions = m_automaton.subdivisionTransitionsOf(m_initStateID.value());
    res.reserve(transitions.size());
    for (TransitionID transitionId : transitions) {
        res.push_back(this->getOperator(transitionId));
    }
    return res;
}

std::pair<std::vector<SubdivisionPoint>, std::vector<SubdivisionPoint>> Bcifs::subdivisionPoints(std::size_t gridLevel) const {
    if (!m_initStateID.has_value() || gridLevel == 1) { return {}; }

    std::vector<SubdivisionPoint> resVar;
    std::vector<SubdivisionPoint> resConst;
    std::unordered_map<StateID, Path> paths = m_automaton.shortestPaths(m_initStateID.value());
    // for each state, look at its subdivisions
    for (const State& state : m_automaton.states()) {
        bool hasMSS = m_mapMSS.find(state.id()) != m_mapMSS.end();
        // if the state has a mass spring system
        if (hasMSS && state.id() != m_initStateID.value()) {
            for (const std::pair<const StateID, MassSpringSystem>& keyval : m_mapMSS) {
                auto it = paths.find(keyval.first);
                if (it != paths.end() && (gridLevel == 0 || gridLevel == it->second.size() + 1)) {
                    std::vector<Mass> masses = keyval.second.masses();
                    for (Mass& mass : masses) {
                        FormalMatrix& posBarycentricSpace = mass.position();
                        bool fixed = true;
                        for (std::size_t i = 0; i < posBarycentricSpace.rows(); i++) {
                            if (posBarycentricSpace.isVar(i, 0)) {
                                fixed = false;
                            }
                        }
                        arma::mat op = this->getOperatorOfPathForMSS(paths[keyval.first]);
                        if (fixed) {
                            resConst.emplace_back(std::move(op), posBarycentricSpace);
                        } else {
                            resVar.emplace_back(std::move(op), posBarycentricSpace);
                        }
                    }
                }
            }
        }
    }
    return { resVar, resConst };
}

std::vector<std::pair<glm::vec3, glm::vec3>> Bcifs::springs(std::size_t gridLevel) const {
    if (!m_initStateID.has_value() || gridLevel == 1) { return {}; }

    std::vector<std::pair<glm::vec3, glm::vec3>> res;
    std::unordered_map<StateID, Path> paths = m_automaton.shortestPaths(m_initStateID.value());
    for (const std::pair<const StateID, MassSpringSystem>& keyval : m_mapMSS) {
        auto it = paths.find(keyval.first);
        if (it != paths.end() && (gridLevel == 0 || gridLevel == it->second.size() + 1)) {
            std::vector<Spring> springs = keyval.second.springs();
            for (Spring& spring : springs) {
                arma::mat op = this->getOperatorOfPathForMSS(paths[keyval.first]);
                arma::mat pos1BarycentricSpace = spring.m1().position().toMat();
                arma::mat pos13D = op * pos1BarycentricSpace;

                arma::mat pos2BarycentricSpace = spring.m2().position().toMat();
                arma::mat pos23D = op * pos2BarycentricSpace;
                res.emplace_back(
                    glm::vec3(pos13D.at(0, 0), pos13D.at(1, 0), pos13D.at(2, 0)),
                    glm::vec3(pos23D.at(0, 0), pos23D.at(1, 0), pos23D.at(2, 0)));
            }
        }
    }

    return res;
}

std::vector<std::pair<glm::vec3, glm::vec3>> Bcifs::controlPointsSprings(std::size_t gridLevel) const {
    if (!m_initStateID.has_value() || gridLevel > 1) { return {}; }

    std::vector<std::pair<glm::vec3, glm::vec3>> res;
    std::vector<Spring> springs = m_MSSControlPoints.springs();
    for (Spring& spring : springs) {
        FormalMatrix pos13D = spring.m1().position();
        FormalMatrix pos23D = spring.m2().position();
        res.emplace_back(
            glm::vec3(pos13D.value(0, 0), pos13D.value(1, 0), pos13D.value(2, 0)),
            glm::vec3(pos23D.value(0, 0), pos23D.value(1, 0), pos23D.value(2, 0)));
    }

    return res;
}

void Bcifs::updateMSS() {
    for (std::pair<const StateID, MassSpringSystem>& keyval : m_mapMSS) {
        keyval.second.update();
        std::vector<TransitionID> transitions = m_automaton.subdivisionTransitionsOf(keyval.first);
        for (TransitionID id : transitions) {
            m_mapOperators.at(id).setSumToOne();
        }
    }
    this->invalidate();
}

void Bcifs::updateMSSControlPoints() {
    m_MSSControlPoints.update();
    this->invalidate(true);
}

void Bcifs::printMSS() const {
    for (const std::pair<const StateID, MassSpringSystem>& keyval : m_mapMSS) {
        Core::LOG_INFO(keyval.second.toString());
    }
    Core::LOG_INFO(m_MSSControlPoints.toString());
}

void Bcifs::invalidate(bool controlPointsOnly) {
    if (!controlPointsOnly) {
        m_invalidatedMatrices = true;
    }
    m_invalidatedMatricesControlPoints = true;
}

void Bcifs::setColorDepth(std::size_t colorDepth) {
    m_colorDepth = colorDepth;
}

Bcifs::ConstraintType Bcifs::constraintType(const Constraint& constraint) const {
    // if the constraint contains at least one subdivision operator, the constraint acts it
    if (isSubdivisionConstraint(constraint)) {
        return ConstraintType::SUBDIVISION;
    }
    // at this stage, the constraint contains only permutation, intern or boundary operators
    // the constraint is a permutation constraint (act on permutation operator) if there is
    // a permutation operator for the first transition of the constraint
    if (m_automaton.findTransitionByID(constraint.first[0]).type() == TransitionType::PERMUTATION ||
        m_automaton.findTransitionByID(constraint.second[0]).type() == TransitionType::PERMUTATION) {
        return ConstraintType::PERMUTATION;
    }
    // else, it is an adjacency on incidence operators
    return ConstraintType::ADJACENCY_ON_INCIDENCE_OPERATORS;
}

bool Bcifs::isSubdivisionConstraint(const Constraint& constraint) const {
    return m_automaton.containsSubdivision(constraint.first) || m_automaton.containsSubdivision(constraint.second);
}

TransitionID Bcifs::addInternal(std::string name, StateID stateID) {
    return this->addTransition(std::move(name), stateID, stateID, TransitionType::INTERNAL);
}

TransitionID Bcifs::addTransition(std::string name, StateID from, StateID to, TransitionType type) {
    Transition transition(m_automaton.transitions().size(), std::move(name), from, to, type);
    m_automaton.addTransition(transition);
    return transition.id();
}

std::string Bcifs::printConstraint(const Constraint& constraint) const {
    std::string res = "[ ";
    bool firstTransition = true;
    for (const auto& transition : constraint.first) {
        const Transition& trans = m_automaton.findTransitionByID(transition);
        if (firstTransition) {
            res += m_automaton.findStateByID(trans.from()).name();
            firstTransition = false;
        }
        res += " ";
        res += trans.toString();
        res += " " + m_automaton.findStateByID(trans.to()).name();
    }
    res += " ] = [ ";
    firstTransition = true;
    for (const auto& transition : constraint.second) {
        const Transition& trans = m_automaton.findTransitionByID(transition);
        if (firstTransition) {
            res += m_automaton.findStateByID(trans.from()).name();
            firstTransition = false;
        }
        res += " ";
        res += trans.toString();
        res += " " + m_automaton.findStateByID(trans.to()).name();
    }
    res += " ]\n";
    return res;
}

void Bcifs::checkAutomaton() const {
    m_automaton.check();
    Core::LOG_DEBUG("Automaton checked");
}

void Bcifs::checkSpaces() const {
    for (const State& state : m_automaton.states()) {
        if (auto it = m_mapSpaces.find(state.id()); it != m_mapSpaces.end()) {
            StateID id = it->first;
            const std::vector<TransitionID>& space = it->second;
            std::vector<TransitionID> boundaries = m_automaton.boundaryTransitionsOf(id);
            for (TransitionID transitionID : space) {
                const Transition& transition = m_automaton.findTransitionByID(transitionID);
                if (transition.type() == TransitionType::INTERNAL) {
                    if (transition.from() != state.id()) {
                        throw std::runtime_error("Space not valid, internal transition " + transition.name() + " does not exist for state " + state.name());
                    }
                } else {
                    if (transition.from() != id) {
                        throw std::runtime_error("Space not valid, boundary " + transition.name() + " does not exist for state " + state.name());
                    }
                }
            }
        }
    }
    Core::LOG_DEBUG("Spaces checked");
}

void Bcifs::checkConstraints() const {
    for (const Constraint& constraint : m_constraints) {
        for (TransitionID id : constraint.first) {
            [[maybe_unused]] const Transition& _ = m_automaton.findTransitionByID(id);
        }
        for (TransitionID id : constraint.second) {
            [[maybe_unused]] const Transition& _ = m_automaton.findTransitionByID(id);
        }
    }
    Core::LOG_DEBUG("Constraints checked");
}

void Bcifs::initializeMatrices() {
    for (const State& state : m_automaton.states()) {
        this->initializeMatrices(state.id());
    }
}

void Bcifs::initializeMatrices(StateID id) {
    // if already initialized, no need to continue
    if (m_mapDimensions.find(id) != m_mapDimensions.end()) { return; }

    // compute the dimension of the state (ignoring adjacency on incidence operators)
    std::size_t dim = m_automaton.internalDimensions(id);
    for (StateID boundaryId : m_automaton.boundaryStatesOf(id)) {
        this->initializeMatrices(boundaryId);
        dim += m_mapDimensions[boundaryId];
    }

    // initialize temporary boundary operators
    std::unordered_map<TransitionID, BooleanMatrix> tempOperators;
    std::size_t lastIndex = 0;
    for (TransitionID transitionId : m_automaton.boundaryTransitionsOf(id)) {
        const Transition& transition = m_automaton.findTransitionByID(transitionId);
        tempOperators.insert({ transitionId, { dim, m_mapDimensions[transition.to()] } });
        // insert identity matrix from lastIndex
        for (std::size_t i = 0; i < m_mapDimensions[transition.to()]; i++) {
            tempOperators[transitionId].set(lastIndex + i, i, true);
        }
        Core::LOG_DEBUG("Temp operator init of " + transition.toString() + " for state " + m_automaton.findStateByID(id).name() + " is:");
        tempOperators[transitionId].print();
        lastIndex += m_mapDimensions[transition.to()];
    }

    // initialize temporary internal operators
    for (TransitionID internalID : m_automaton.internalTransitionsOf(id)) {
        tempOperators.insert({ internalID, { dim, 1 } });
        // insert 1 at lastIndex row
        tempOperators[internalID].set(lastIndex, 0, true);
        Core::LOG_DEBUG("Temp operator init of " + m_automaton.findTransitionByID(internalID).toString() + " for state " + m_automaton.findStateByID(id).name() + " is:");
        tempOperators[internalID].print();
        lastIndex++;
    }

    // initialize equivalence relation matrix with identity
    BooleanMatrix M(dim, dim);
    M.setIdentity();
    Core::LOG_DEBUG("Init M matrix of state " + m_automaton.findStateByID(id).name() + " is:");
    M.print();
    for (const Constraint& constraint : m_adjacencyConstraintsOnIncidenceOperators) {
        if (m_automaton.findTransitionByID(constraint.first[0]).from() == id && m_automaton.findTransitionByID(constraint.second[0]).from() == id) {
            BooleanMatrix lhs = tempOperators[constraint.first[0]];
            for (std::size_t i = 1; i < constraint.first.size(); i++) {
                lhs = lhs * m_mapOperators.at(constraint.first[i]).toBooleanMatrix();
            }
            BooleanMatrix rhs = tempOperators[constraint.second[0]];
            for (std::size_t i = 1; i < constraint.second.size(); i++) {
                rhs = rhs * m_mapOperators.at(constraint.second[i]).toBooleanMatrix();
            }
            // at this stage, lhs and rhs must be the same,
            // so we initialize column by column the link in the graph matrix M
            for (std::size_t col = 0; col < lhs.cols(); col++) {
                std::size_t indexLhs = lhs.lineOfTrueInColumn(col);
                std::size_t indexRhs = rhs.lineOfTrueInColumn(col);
                // set directly the M matrix symmetric
                M.set(indexLhs, indexRhs, true);
                M.set(indexRhs, indexLhs, true);
            }
        }
    }

    // set M transitive
    M = M.transitived();
    Core::LOG_DEBUG("For state " + m_automaton.findStateByID(id).name() + ", the M matrix is:");
    M.print();

    // remove multiple rows of M to get projection matrix
    BooleanMatrix proj = M.removedMultipleRows();
    Core::LOG_DEBUG("For state " + m_automaton.findStateByID(id).name() + ", the projection matrix is:");
    proj.print();

    // set the dimension of the current state
    m_mapDimensions[id] = proj.rows();

    // set final boundary and internal operators
    for (TransitionID transitionId : m_automaton.boundaryAndInternalTransitionsOf(id)) {
        BooleanMatrix boundaryOperatorBool = proj * tempOperators[transitionId];
        FormalMatrix boundaryOperator = boundaryOperatorBool.toFormalMatrix(this->pool());
        m_mapOperators.insert({ transitionId, boundaryOperator });
        Core::LOG_DEBUG("Operator of " + m_automaton.findTransitionByID(transitionId).toString() + " for state " + m_automaton.findStateByID(id).name() + " is:");
        boundaryOperatorBool.print();
    }

    // take space into account
    if (m_mapSpaces.find(id) != m_mapSpaces.end()) {
        // find number of column for the permutation space matrix
        std::size_t nbCols = 0;
        for (TransitionID transitionId : m_mapSpaces[id]) {
            nbCols += m_mapOperators.at(transitionId).cols();
        }

        // init the permutation space matrix by concatenating the values of boundary transitions specified in the space
        BooleanMatrix permutationSpace(proj.rows(), nbCols);
        std::size_t lastColumnIndex = 0;
        for (TransitionID transitionId : m_mapSpaces[id]) {
            for (std::size_t col = 0; col < m_mapOperators.at(transitionId).cols(); col++) {
                for (std::size_t row = 0; row < m_mapOperators.at(transitionId).rows(); row++) {
                    permutationSpace.set(row, col + lastColumnIndex, m_mapOperators.at(transitionId).isOne(row, col));
                }
            }
            lastColumnIndex += m_mapOperators.at(transitionId).cols();
        }
        Core::LOG_DEBUG("For state " + m_automaton.findStateByID(id).name() + ", the permutation space matrix is:");
        permutationSpace.print();

        // remove multiple columns of the permutation space matrix
        permutationSpace = permutationSpace.removedMultipleCols();
        Core::LOG_DEBUG("after column simplification, it is:");
        permutationSpace.print();

        // square the matrix and fill by true to have a 1 per line and per column
        permutationSpace.squareAndFillByTrue();
        Core::LOG_DEBUG("and after fill by true to complete space, it is:");
        permutationSpace.print();

        // transpose the matrix
        permutationSpace = permutationSpace.transposed();
        Core::LOG_DEBUG("and after transpose, it is:");
        permutationSpace.print();

        // apply this matrix to all boundary and internal operators to update them
        Core::LOG_DEBUG("Apply this matrix to all boundary and internal operators to update them");
        for (TransitionID transitionId : m_automaton.boundaryAndInternalTransitionsOf(id)) {
            m_mapOperators.at(transitionId) = permutationSpace.toFormalMatrix(this->pool()) * m_mapOperators.at(transitionId);
            Core::LOG_DEBUG("Operator of " + m_automaton.findTransitionByID(transitionId).toString() + " for state " + m_automaton.findStateByID(id).name() + " is:");
            m_mapOperators.at(transitionId).printDebug();
        }
    }

    // resolve permutation constraints to initialize permutation matrices
    this->resolvePermutationConstraints(id);
}

void Bcifs::resolvePermutationConstraints(StateID id) {
    Core::LOG_DEBUG("Resolving permutation constraints...");
    for (const Constraint& constraint : m_permutationConstraints) {
        const Transition& firstTransLeft = m_automaton.findTransitionByID(constraint.first[0]);
        const Transition& firstTransRight = m_automaton.findTransitionByID(constraint.second[0]);
        if (firstTransLeft.from() == id && firstTransRight.from() == id) {
            FormalMatrix lhs = this->getOrInitOperator(constraint.first[0]);
            Core::LOG_DEBUG("lhs is:");
            lhs.printDebug(true);
            for (std::size_t i = 1; i < constraint.first.size(); i++) {
                Core::LOG_DEBUG("next matrix is:");
                this->getOrInitOperator(constraint.first[i]).printDebug();
                lhs = lhs * this->getOrInitOperator(constraint.first[i]);
                Core::LOG_DEBUG("new lhs is:");
                lhs.printDebug(true);
            }
            FormalMatrix rhs = this->getOrInitOperator(constraint.second[0]);
            Core::LOG_DEBUG("rhs is:");
            rhs.printDebug(true);
            for (std::size_t i = 1; i < constraint.second.size(); i++) {
                Core::LOG_DEBUG("next matrix is:");
                this->getOrInitOperator(constraint.second[i]).printDebug();
                rhs = rhs * this->getOrInitOperator(constraint.second[i]);
                Core::LOG_DEBUG("new rhs is:");
                rhs.printDebug(true);
            }
            ConstraintSolver::solve(lhs, rhs, m_pool);
        }
    }
    Core::LOG_DEBUG("Resolved all permutation constraints.");
}

void Bcifs::resolveConstraints() {
    Core::LOG_DEBUG("Resolving constraints...");
    for (const Constraint& constraint : m_constraints) {
        Core::LOG_DEBUG("Constraint before solve:");
        this->printConstraintMatrices(constraint);

        FormalMatrix lhs = this->getOrInitOperator(constraint.first[0]);
        for (std::size_t i = 1; i < constraint.first.size(); i++) {
            lhs = lhs * this->getOrInitOperator(constraint.first[i]);
        }
        Core::LOG_DEBUG("lhs is:");
        lhs.printDebug(true);

        FormalMatrix rhs = this->getOrInitOperator(constraint.second[0]);
        for (std::size_t i = 1; i < constraint.second.size(); i++) {
            rhs = rhs * this->getOrInitOperator(constraint.second[i]);
        }
        Core::LOG_DEBUG("rhs is:");
        rhs.printDebug(true);

        ConstraintSolver::solve(lhs, rhs, m_pool);

        Core::LOG_DEBUG("Constraint after solve:");
        this->printConstraintMatrices(constraint);
    }
    Core::LOG_DEBUG("Resolved all constraints.");
}

void Bcifs::initSubdivisionOperators() {
    for (const Transition& transition : m_automaton.transitions()) {
        auto itOperator = m_mapOperators.find(transition.id());
        if (itOperator == m_mapOperators.end()) {
            // if the matrix does not exist, then it is an undefined matrix, so we can initialize it with a random value
            // 2.0f is chosen for debug info
            m_mapOperators.insert({ transition.id(), FormalMatrix(m_mapDimensions[transition.from()], m_mapDimensions[transition.to()], 2.f, this->pool()) });
        }
    }
}

const FormalMatrix& Bcifs::getOrInitOperator(TransitionID id) {
    auto itOperator = m_mapOperators.find(id);
    if (itOperator == m_mapOperators.end()) {
        // if the matrix does not exist, then it is an undefined matrix, so we can initialize it with a random value
        // 2.0f is chosen for debug info
        const Transition& transition = m_automaton.findTransitionByID(id);
        m_mapOperators.insert({ id, FormalMatrix(m_mapDimensions[transition.from()], m_mapDimensions[transition.to()], 2.f, this->pool()) });
    }
    return m_mapOperators.at(id);
}

const FormalMatrix& Bcifs::getOperator(TransitionID id) const {
    return m_mapOperators.find(id)->second;
}

const arma::mat& Bcifs::getOperatorMat(TransitionID id) const {
    return m_mapOperatorsMat.find(id)->second;
}

void Bcifs::printConstraintMatrices(const Constraint& constraint) {
    Core::LOG_DEBUG("LHS:");
    for (TransitionID transitionId : constraint.first) {
        const Transition& transition = m_automaton.findTransitionByID(transitionId);
        Core::LOG_DEBUG(transition.toString() + " from " + m_automaton.findStateByID(transition.from()).name() + " to " + m_automaton.findStateByID(transition.to()).name() + " is:");
        this->getOrInitOperator(transitionId).printDebug(true);
    }
    Core::LOG_DEBUG("RHS:");
    for (TransitionID transitionId : constraint.second) {
        const Transition& transition = m_automaton.findTransitionByID(transitionId);
        Core::LOG_DEBUG(transition.toString() + " from " + m_automaton.findStateByID(transition.from()).name() + " to " + m_automaton.findStateByID(transition.to()).name() + " is:");
        this->getOrInitOperator(transitionId).printDebug(true);
    }
}

void Bcifs::completeSubdivisionMatrices() {
    for (const Transition& transition : m_automaton.transitions()) {
        if (transition.type() == TransitionType::SUBDIVISION) {
            if (m_mapInitMat.find(transition.id()) == m_mapInitMat.end()) {
                m_mapOperators.at(transition.id()).setRandomValuesOnFreeCoefs(transition.from() != m_initStateID.value());
            } else {
                FormalMatrix& operatorMat = m_mapOperators.at(transition.id());
                FormalMatrix& initMat = m_mapInitMat.at(transition.id());
                if (operatorMat.rows() == initMat.rows() && operatorMat.cols() == initMat.cols()) {
                    for (std::size_t row = 0; row < operatorMat.rows(); row++) {
                        for (std::size_t col = 0; col < operatorMat.cols(); col++) {
                            if (operatorMat.isVar(row, col)) {
                                m_pool.setValue(operatorMat.get(row, col).index(), initMat.value(row, col));
                                m_pool.setKind(operatorMat.get(row, col).index(), m_pool.getKind(initMat.get(row, col).index()));
                                m_pool.setInitialized(operatorMat.get(row, col).index());
                            }
                        }
                    }
                } else {
                    m_mapOperators.at(transition.id()).setRandomValuesOnFreeCoefs(transition.from() != m_initStateID.value());
                }
            }
        }
    }
}

void Bcifs::buildGridsFromBoundary() {
    for (StateID id : m_createGridFromBoundary) {
        std::vector<TransitionID> boundaries = m_automaton.boundaryTransitionsOf(id);
        std::vector<GridFigure> gridFigures;
        gridFigures.reserve(boundaries.size());
        for (TransitionID transitionId : boundaries) {
            // create as many figures for each figure of each boundary
            Transition transition = m_automaton.findTransitionByID(transitionId);
            const Grid& gridBoundary = m_mapGrids.at(transition.to());
            for (const GridFigure& gridFigureBoundary : gridBoundary) {
                Figure figure = gridFigureBoundary.paths(); // copy
                for (Path& path : figure) {
                    path.insert(path.begin(), transitionId);
                }
                gridFigures.emplace_back(std::move(figure), gridFigureBoundary.k(), gridFigureBoundary.length());
            }
        }
        m_mapGrids.emplace(id, gridFigures);
    }
}

void Bcifs::buildMassSpringSystems() {
    std::unordered_map<StateID, Path> paths = m_automaton.shortestPaths(m_initStateID.value());
    // for each state, look at its subdivisions
    for (const State& state : m_automaton.states()) {
        bool needMSS = false;
        // if current state is accessible
        if (paths.find(state.id()) != paths.end()) {
            for (TransitionID transitionId : m_automaton.subdivisionTransitionsOf(state.id())) {
                const Transition& transition = m_automaton.findTransitionByID(transitionId);
                // if arrival state has a grid
                if (m_mapGrids.find(transition.to()) != m_mapGrids.end()) {
                    needMSS = true;
                }
            }
        }
        // if at least one of the arrival state has a grid
        if (needMSS && state.id() != m_initStateID.value()) {
            // then build a mss for the current state (so, in the dimension of the current state and not the arrival state)
            m_mapMSS[state.id()].clear(m_mapDimensions[state.id()]);
            // at first, build a global formal matrix by concatenating all subdivision operators
            // simplify the global matrix by removing all identical columns
            FormalMatrix globalMatrix = this->globalMatrixOf(state.id());
            // for each column in the global matrix
            for (std::size_t i = 0; i < globalMatrix.cols(); i++) {
                // create a mass
                m_mapMSS[state.id()].addMass(globalMatrix.getCol(i), m_damping);
            }
            std::size_t lastMassIndex = 0;
            // for each subdivision operator
            for (TransitionID transitionId : m_automaton.subdivisionTransitionsOf(state.id())) {
                const Transition& transition = m_automaton.findTransitionByID(transitionId);
                if (m_mapGrids.find(transition.to()) != m_mapGrids.end()) {
                    // for each Figure of the grid of the arrival state
                    for (const GridFigure& gridFigure : m_mapGrids[transition.to()]) {
                        bool firstMass = true;
                        // for each consecutive Path of the Figure
                        for (const Path& path : gridFigure.paths()) {
                            // identify the column index of the Paths in the global matrix
                            // add a spring between the 2 masses at the found indices
                            FormalMatrix matrix = this->getOperatorOfPathNoSubdivision(path);
                            // always assume it is a one column matrix
                            FormalMatrix finalColumn = m_mapOperators.at(transitionId) * matrix;
                            std::size_t index = globalMatrix.indexOf(finalColumn);
                            if (!firstMass) {
                                float k = gridFigure.k() < 0.0f ? m_k : gridFigure.k();
                                m_mapMSS[state.id()].addSpring(lastMassIndex, index, k, 0.0f);
                            }
                            firstMass = false;
                            lastMassIndex = index;
                        }
                    }
                }
            }
        }
    }
}

void Bcifs::buildMSSForControlPoints() {
    // build a mss for the init state (so, in the dimension of the init state and not the arrival state)
    m_MSSControlPoints.clear(m_mapDimensions[m_initStateID.value()]);
    // at first, build a global formal matrix by concatenating all subdivision operators
    // simplify the global matrix by removing all identical columns
    FormalMatrix globalMatrix = this->globalMatrixOf(m_initStateID.value());
    // for each column in the global matrix
    for (std::size_t i = 0; i < globalMatrix.cols(); i++) {
        // create a mass
        m_MSSControlPoints.addMass(globalMatrix.getCol(i), m_dampingControlPoints);
    }
    std::size_t lastMassIndex = 0;
    // for each subdivision operator
    for (TransitionID transitionId : m_automaton.subdivisionTransitionsOf(m_initStateID.value())) {
        const Transition& transition = m_automaton.findTransitionByID(transitionId);
        if (m_mapGrids.find(transition.to()) != m_mapGrids.end()) {
            // for each Figure of the grid of the arrival state
            for (const GridFigure& gridFigure : m_mapGrids[transition.to()]) {
                bool firstMass = true;
                // for each consecutive Path of the Figure
                for (const Path& path : gridFigure.paths()) {
                    // identify the column index of the Paths in the global matrix
                    // add a spring between the 2 masses at the found indices
                    FormalMatrix matrix = this->getOperatorOfPathNoSubdivision(path);
                    // always assume it is a one column matrix
                    FormalMatrix finalColumn = m_mapOperators.at(transitionId) * matrix;
                    std::size_t index = globalMatrix.indexOf(finalColumn);
                    if (!firstMass) {
                        float k = gridFigure.k() < 0.0f ? m_k : gridFigure.k();
                        float length = gridFigure.length() < 0.0f ? m_lengthControlPoints : gridFigure.length();
                        m_MSSControlPoints.addSpring(lastMassIndex, index, k, length);
                    }
                    firstMass = false;
                    lastMassIndex = index;
                }
            }
        }
    }
    m_MSSControlPoints.createAngularSprings(m_kControlPoints / 2.0f);
}

arma::mat Bcifs::getOperatorOfPath(const Path& path) const {
    arma::mat res = this->getOperatorMat(path[1]);
    for (std::size_t i = 2; i < path.size(); i++) {
        res *= this->getOperatorMat(path[i]);
    }
    return res;
}

arma::mat Bcifs::getOperatorOfPathForPrimitive(const Path& path) const {
    arma::mat res = this->getOperatorMat(path[0]);
    for (std::size_t i = 1; i < path.size(); i++) {
        res *= this->getOperatorMat(path[i]);
    }
    return res;
}

arma::mat Bcifs::getOperatorOfPathForMSS(const Path& path) const {
    arma::mat res = this->getOperator(path[0]).toMat();
    for (std::size_t i = 1; i < path.size(); i++) {
        res = res * this->getOperator(path[i]).toMat();
    }
    return res;
}

FormalMatrix Bcifs::getOperatorOfPathNoSubdivision(const Path& path) const {
    FormalMatrix res = this->getOperator(path[0]);
    for (std::size_t i = 1; i < path.size(); i++) {
        res = res * this->getOperator(path[i]);
    }
    return res;
}

FormalMatrix Bcifs::globalMatrixOf(StateID id) const {
    std::vector<TransitionID> transitions = m_automaton.subdivisionTransitionsOf(id);
    if (transitions.size() == 0) { throw std::runtime_error("There is no subdivision transitions on the state " + m_automaton.findStateByID(id).name()); }
    FormalMatrix res = this->getOperator(transitions[0]);
    for (std::size_t i = 1; i < transitions.size(); i++) {
        res.concatenateColumns(this->getOperator(transitions[i]));
    }
    return res;
}

const std::vector<arma::mat>& Bcifs::getPrimitiveMat(StateID id) const {
    return m_mapPrimitivesMat.find(id)->second;
}

void Bcifs::initPrimitives() {
    // for each state, use the given primitive or init a defaut one
    // then compute the arma matrix and store it
    for (const State& state : m_automaton.states()) {
        auto it = m_mapPrimitives.find(state.id());
        if (it != m_mapPrimitives.end()) {
            m_mapPrimitivesMat[state.id()] = {};
            for (const Figure& figure : it->second) {
                // each figure is a face, each face is a path, that has a matrix associated,
                // each column is a point of the primitive
                // for now we assume each path gives a one-column matrix
                arma::mat primitive(m_mapDimensions[state.id()], figure.size());
                std::size_t currentCol = 0;
                for (const Path& path : figure) {
                    if (m_automaton.containsSubdivision(path)) {
                        m_needUpdatePrimitivesWhenChangingMatrices = true;
                    }
                    arma::mat mat = this->getOperatorOfPathForPrimitive(path);
                    primitive.col(currentCol) = mat;
                    currentCol++;
                }
                m_mapPrimitivesMat[state.id()].push_back(std::move(primitive));
            }
        } else {
            // init a default primitive, the identity matrix
            arma::mat identity(m_mapDimensions[state.id()], m_mapDimensions[state.id()], arma::fill::eye);
            m_mapPrimitivesMat[state.id()] = { identity };
        }
    }
}

const glm::vec3& Bcifs::getFrontColor(const Path& path) const {
    if (m_colorDepth < path.size()) {
        auto it = m_frontColors.find(path[m_colorDepth]);
        if (it != m_frontColors.end()) {
            return it->second;
        }
        return m_defaultFrontColor;
    }
    return m_defaultFrontColor;
}

const glm::vec3& Bcifs::getBackColor(const Path& path) const {
    if (m_colorDepth < path.size()) {
        auto it = m_backColors.find(path[m_colorDepth]);
        if (it != m_backColors.end()) {
            return it->second;
        }
        return m_defaultBackColor;
    }
    return m_defaultBackColor;
}

} // BCIFS
