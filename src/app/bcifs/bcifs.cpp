#include <iostream>
#include "app/bcifs/bcifs.h"
#include "app/automaton/state.h"
#include "app/bcifs/constraintsolver.h"
#include <algorithm>

#include "app/bcifs/booleanmatrix.h"

namespace BCIFS {

std::pair<StateID, std::vector<TransitionID>> Bcifs::addState(std::string name, std::size_t internalDimensions) {
    State s(m_automaton.states().size(), std::move(name));
    m_automaton.addState(s);
    std::vector<TransitionID> internalTransitions(internalDimensions);
    for (std::size_t i = 0; i < internalDimensions; i++) {
        internalTransitions[i] = this->addInternal(std::to_string(i), s.id());
    }
    return { s.id(), std::move(internalTransitions) };
}

StateID Bcifs::addInitState() {
    auto [id, _] = this->addState("#", 3);
    m_initStateID.emplace(id);
    return id;
}

TransitionID Bcifs::addBoundary(std::string name, StateID from, StateID to) {
    return this->addTransition(std::move(name), from, to, TransitionType::BOUNDARY);
}

void Bcifs::setSpace(StateID id, std::vector<TransitionID> transitions) {
    m_mapSpaces[id] = std::move(transitions);
}

TransitionID Bcifs::addSubdivision(std::string name, StateID from, StateID to) {
    return this->addTransition(std::move(name), from, to, TransitionType::SUBDIVISION);
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

void Bcifs::print() const {
    m_automaton.print();
    std::cout << "Classic constraints:\n";
    for (const auto& constraint: m_constraints) {
        this->printConstraint(constraint);
    }
    std::cout << "\nAdjacency constraints on incidence operators:\n";
    for (const auto& constraint: m_adjacencyConstraintsOnIncidenceOperators) {
        this->printConstraint(constraint);
    }
    std::cout << "\nDimensions of states:\n";
    for (const std::pair<StateID, std::size_t> keyval: m_mapDimensions) {
        std::cout << m_automaton.findStateByID(keyval.first).name() << " has " << keyval.second << " dimensions\n";
    }
    std::cout << "\nAll operators:\n";
    for (const auto& keyval: m_mapOperators) {
        TransitionID transitionId = keyval.first;
        const Transition& transition = m_automaton.findTransitionByID(transitionId);
        std::cout << "Operator ";
        transition.print();
        std::cout << " from " << m_automaton.findStateByID(transition.from()).name();
        std::cout << " to " << m_automaton.findStateByID(transition.to()).name() << "\n";
        if (transition.type() == TransitionType::SUBDIVISION) {
            keyval.second.print(true);
            std::cout << "-------\n";
            keyval.second.print();
        } else {
            keyval.second.print();
        }
    }
    std::cout.flush();
}

void Bcifs::validate() {
    //TODO: try catch all this block to stop when there is a problem and signal it to the user without crashing
    this->checkAutomaton(); // all states used in transitions exist
    this->checkSpaces(); // all states have a valid space
    this->checkConstraints(); // same arrival (existing) state for each path
    this->initializeMatrices(); // initialize all operators
    this->resolveConstraints(); // resolve all constraints to finish the matrices initialization
    this->completeSubdvisionMatrices(); // make sure matrices are barycentric transformations
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
    return m_automaton.containsSubdvision(constraint.first) || m_automaton.containsSubdvision(constraint.second);
}

TransitionID Bcifs::addInternal(std::string name, StateID stateID) {
    return this->addTransition(std::move(name), stateID, stateID, TransitionType::INTERNAL);
}

TransitionID Bcifs::addTransition(std::string name, StateID from, StateID to, TransitionType type) {
    Transition transition(m_automaton.transitions().size(), std::move(name), from, to, type);
    m_automaton.addTransition(transition);
    return transition.id();
}

void Bcifs::printConstraint(const Constraint& constraint) const {
    std::cout << "[ ";
    bool firstTransition = true;
    for (const auto& transition: constraint.first) {
        const Transition& trans = m_automaton.findTransitionByID(transition);
        if (firstTransition) {
            std::cout << m_automaton.findStateByID(trans.from()).name();
            firstTransition = false;
        }
        std::cout << " ";
        trans.print();
        std::cout << " " << m_automaton.findStateByID(trans.to()).name();
    }
    std::cout << " ] = [ ";
    firstTransition = true;
    for (const auto& transition: constraint.second) {
        const Transition& trans = m_automaton.findTransitionByID(transition);
        if (firstTransition) {
            std::cout << m_automaton.findStateByID(trans.from()).name();
            firstTransition = false;
        }
        std::cout << " ";
        trans.print();
        std::cout << " " << m_automaton.findStateByID(trans.to()).name();
    }
    std::cout << " ]\n";
}

void Bcifs::checkAutomaton() const {
    m_automaton.check();
    std::cout << "Automaton checked" << std::endl;
}

void Bcifs::checkSpaces() const {
    for (const State& state: m_automaton.states()) {
        if (auto it = m_mapSpaces.find(state.id()); it != m_mapSpaces.end()) {
            StateID id = it->first;
            const std::vector<TransitionID>& space = it->second;
            std::vector<TransitionID> boundaries = m_automaton.boundaryTransitionOf(id);
            for (TransitionID transitionID: space) {
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
    std::cout << "Spaces checked" << std::endl;
}

void Bcifs::checkConstraints() const {
    for (const Constraint& constraint: m_constraints) {
        for (TransitionID id: constraint.first) {
            m_automaton.findTransitionByID(id);
        }
        for (TransitionID id: constraint.second) {
            m_automaton.findTransitionByID(id);
        }
    }
    std::cout << "Constraints checked" << std::endl;
}

void Bcifs::initializeMatrices() {
    for (const State& state: m_automaton.states()) {
        this->initializeMatrices(state.id());
    }
}

void Bcifs::initializeMatrices(StateID id) {
    // if already initialized, no need to continue
    if (m_mapDimensions.find(id) != m_mapDimensions.end()) { return; }

    // compute the dimension of the state (ignoring adjacency on incidence operators)
    std::size_t dim = m_automaton.internalDimensions(id);
    for (StateID boundaryId: m_automaton.boundaryStateOf(id)) {
        this->initializeMatrices(boundaryId);
        dim += m_mapDimensions[boundaryId];
    }

    // initialize temporary boundary operators
    std::unordered_map<TransitionID, BooleanMatrix> tempOperators;
    std::size_t lastIndex = 0;
    for (TransitionID transitionId: m_automaton.boundaryTransitionOf(id)) {
        const Transition& transition = m_automaton.findTransitionByID(transitionId);
        tempOperators.insert({ transitionId, { dim, m_mapDimensions[transition.to()] }});
        // insert identity matrix from lastIndex
        for (std::size_t i = 0; i < m_mapDimensions[transition.to()]; i++) {
            tempOperators[transitionId].set(lastIndex + i, i, true);
        }
        std::cout << "Temp operator init of ";
        transition.print();
        std::cout << " for state " << m_automaton.findStateByID(id).name() << " is:\n";
        tempOperators[transitionId].print();
        std::cout.flush();
        lastIndex += m_mapDimensions[transition.to()];
    }

    // initialize temporary internal operators
    for (TransitionID internalID: m_automaton.internalTransitionOf(id)) {
        tempOperators.insert({ internalID, { dim, 1 }});
        // insert 1 at lastIndex row
        tempOperators[internalID].set(lastIndex, 0, true);
        std::cout << "Temp operator init of ";
        m_automaton.findTransitionByID(internalID).print();
        std::cout << " for state " << m_automaton.findStateByID(id).name() << " is:\n";
        tempOperators[internalID].print();
        lastIndex++;
    }

    // initialize equivalence relation matrix with identity
    BooleanMatrix M(dim, dim);
    M.setIdentity();
    std::cout << "Init M matrix of state " << m_automaton.findStateByID(id).name() << " is:\n";
    M.print();
    std::cout.flush();
    for (const Constraint& constraint: m_adjacencyConstraintsOnIncidenceOperators) {
        if (m_automaton.findTransitionByID(constraint.first[0]).from() == id && m_automaton.findTransitionByID(constraint.second[0]).from() == id) {
            BooleanMatrix lhs = tempOperators[constraint.first[0]];
            for (std::size_t i = 1; i < constraint.first.size(); i++) {
                lhs = lhs * m_mapOperators[constraint.first[i]].toBooleanMatrix();
            }
            BooleanMatrix rhs = tempOperators[constraint.second[0]];
            for (std::size_t i = 1; i < constraint.second.size(); i++) {
                rhs = rhs * m_mapOperators[constraint.second[i]].toBooleanMatrix();
            }
            // at this stage, lhs and rhs must be the same,
            // so we initialize column by column the link in the graph matrix M
            for (std::size_t col = 0; col < lhs.cols(); col++) {
                std::size_t indexLhs = lhs.lineOfTrueInColumn(col);
                std::size_t indexRhs = rhs.lineOfTrueInColumn(col);
                // set directly the M matrix symetric
                M.set(indexLhs, indexRhs, true);
                M.set(indexRhs, indexLhs, true);
            }
        }
    }

    // set M transitive
    M = M.transitived();
    std::cout << "For state " << m_automaton.findStateByID(id).name() << ", the M matrix is:\n";
    M.print();

    // remove multiple rows of M to get projection matrix
    BooleanMatrix proj = M.removedMultipleRows();
    std::cout << "For state " << m_automaton.findStateByID(id).name() << ", the projection matrix is:\n";
    proj.print();

    // set the dimension of the current state
    m_mapDimensions[id] = proj.rows();

    // set final boundary and internal operators
    for (TransitionID transitionId: m_automaton.boundaryAndInternalTransitionOf(id)) {
        BooleanMatrix boundaryOperatorBool = proj * tempOperators[transitionId];
        FormalMatrix boundaryOperator = boundaryOperatorBool.toFormalMatrix();
        m_mapOperators.insert({ transitionId, boundaryOperator });
        std::cout << "Operator of ";
        const Transition& transition = m_automaton.findTransitionByID(transitionId);
        transition.print();
        std::cout << " for state " << m_automaton.findStateByID(id).name() << " is:\n";
        boundaryOperatorBool.print();
        std::cout.flush();
    }

    // take space into account
    if (m_mapSpaces.find(id) != m_mapSpaces.end()) {
        // find number of column for the permutation space matrix
        std::size_t nbCols = 0;
        for (TransitionID transitionID: m_mapSpaces[id]) {
            nbCols += m_mapOperators[transitionID].cols();
        }

        // init the permutation space matrix by concatenating the values of boundary transitions specified in the space
        BooleanMatrix permutationSpace(proj.rows(), nbCols);
        std::size_t lastColumnIndex = 0;
        for (TransitionID transitionID: m_mapSpaces[id]) {
            for (std::size_t col = 0; col < m_mapOperators[transitionID].cols(); col++) {
                for (std::size_t row = 0; row < m_mapOperators[transitionID].rows(); row++) {
                    permutationSpace.set(row, col + lastColumnIndex, m_mapOperators[transitionID].get(row, col)->type() == CoefType::ONE);
                }
            }
            lastColumnIndex += m_mapOperators[transitionID].cols();
        }
        std::cout << "For state " << m_automaton.findStateByID(id).name() << ", the permutation space matrix is:\n";
        permutationSpace.print();

        // remove multiple columns of the permutation space matrix
        permutationSpace = permutationSpace.removedMultipleCols();
        std::cout << "after column simplification, it is:\n";
        permutationSpace.print();

        // square the matrix and fill by true to have a 1 per line and per column
        permutationSpace.squareAndFillByTrue();
        std::cout << "and after fill by true to complete space, it is:\n";
        permutationSpace.print();

        // transpose the matrix
        permutationSpace = permutationSpace.transposed();
        std::cout << "and after transpose, it is:\n";
        permutationSpace.print();

        // apply this matrix to all boundary and internal operators to update them
        std::cout << "Apply this matrix to all boundary and internal operators to update them" << std::endl;
        for (TransitionID transitionId: m_automaton.boundaryAndInternalTransitionOf(id)) {
            m_mapOperators[transitionId] = permutationSpace.toFormalMatrix() * m_mapOperators[transitionId];
            std::cout << "Operator of ";
            const Transition& transition = m_automaton.findTransitionByID(transitionId);
            transition.print();
            std::cout << " for state " << m_automaton.findStateByID(id).name() << " is:\n";
            m_mapOperators[transitionId].print();
            std::cout.flush();
        }
    }

    // resolve permutation constraints to initialize permutation matrices
    this->resolvePermutationConstraints(id);
}

void Bcifs::resolvePermutationConstraints(StateID id) {
    std::cout << "Resolving permutation constraints...\n";
    for (const Constraint& constraint: m_permutationConstraints) {
        const Transition& firstTransLeft = m_automaton.findTransitionByID(constraint.first[0]);
        const Transition& firstTransRight = m_automaton.findTransitionByID(constraint.second[0]);
        if (firstTransLeft.from() == id && firstTransRight.from() == id) {
            FormalMatrix lhs = this->getOrInitOperator(constraint.first[0]);
            std::cout << "lhs is:\n";
            lhs.print(true);
            for (std::size_t i = 1; i < constraint.first.size(); i++) {
                std::cout << "next matrix is:\n";
                this->getOrInitOperator(constraint.first[i]).print();
                lhs = lhs * this->getOrInitOperator(constraint.first[i]);
                std::cout << "new lhs is:\n";
                lhs.print(true);
            }
            FormalMatrix rhs = this->getOrInitOperator(constraint.second[0]);
            std::cout << "rhs is:\n";
            rhs.print(true);
            for (std::size_t i = 1; i < constraint.second.size(); i++) {
                std::cout << "next matrix is:\n";
                this->getOrInitOperator(constraint.second[i]).print();
                rhs = rhs * this->getOrInitOperator(constraint.second[i]);
                std::cout << "new rhs is:\n";
                rhs.print(true);
            }
            std::cout.flush();
            ConstraintSolver::solve(lhs, rhs);
        }
    }
    std::cout << "Resolved all permutation constraints." << std::endl;
}

void Bcifs::resolveConstraints() {
    std::cout << "Resolving constraints...\n";
    for (const Constraint& constraint: m_constraints) {
        std::cout << "Constraint before solve:\n";
        this->printConstraintMatrices(constraint);

        FormalMatrix lhs = this->getOrInitOperator(constraint.first[0]);
        for (std::size_t i = 1; i < constraint.first.size(); i++) {
            lhs = lhs * this->getOrInitOperator(constraint.first[i]);
        }
        std::cout << "lhs is:\n";
        lhs.print(true);

        FormalMatrix rhs = this->getOrInitOperator(constraint.second[0]);
        for (std::size_t i = 1; i < constraint.second.size(); i++) {
            rhs = rhs * this->getOrInitOperator(constraint.second[i]);
        }
        std::cout << "rhs is:\n";
        rhs.print(true);

        ConstraintSolver::solve(lhs, rhs);

        std::cout << "Constraint after solve:\n";
        this->printConstraintMatrices(constraint);
    }
    std::cout << "Resolved all constraints." << std::endl;
}

const FormalMatrix& Bcifs::getOrInitOperator(TransitionID id) {
    auto itOperator = m_mapOperators.find(id);
    if (itOperator == m_mapOperators.end()) {
        // if the matrix does not exist, then it is an undefined matrix, so we can initialize it with a random value
        // 2.0f is chosen for debug info
        const Transition& transition = m_automaton.findTransitionByID(id);
        m_mapOperators.insert({ id, FormalMatrix(m_mapDimensions[transition.from()], m_mapDimensions[transition.to()], 2.f) });
    }
    return m_mapOperators[id];
}

void Bcifs::printConstraintMatrices(const Bcifs::Constraint& constraint) {
    std::cout << "LHS:\n";
    for (TransitionID transitionId: constraint.first) {
        const Transition& transition = m_automaton.findTransitionByID(transitionId);
        transition.print();
        std::cout << " from " << m_automaton.findStateByID(transition.from()).name();
        std::cout << " to " << m_automaton.findStateByID(transition.to()).name();
        std::cout << " is:\n";
        this->getOrInitOperator(transitionId).print(true);
    }
    std::cout << "\nRHS:\n";
    for (TransitionID transitionId: constraint.second) {
        const Transition& transition = m_automaton.findTransitionByID(transitionId);
        transition.print();
        std::cout << " from " << m_automaton.findStateByID(transition.from()).name();
        std::cout << " to " << m_automaton.findStateByID(transition.to()).name();
        std::cout << " is:\n";
        this->getOrInitOperator(transitionId).print(true);
    }
    std::cout << std::endl;
}

void Bcifs::completeSubdvisionMatrices() {
    for (const Transition& transition: m_automaton.transitions()) {
        if (transition.type() == TransitionType::SUBDIVISION) {
            m_mapOperators[transition.id()].setRandomValuesOnFreeCoefs();
        }
    }
}

} // BCIFS