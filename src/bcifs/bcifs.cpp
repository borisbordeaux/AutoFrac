#include <iostream>
#include "bcifs/bcifs.h"
#include "automaton/state.h"

namespace BCIFS {

std::pair<StateID, std::vector<TransitionID>> Bcifs::addState(std::string name, std::size_t internalDimensions) {
    State s(m_automaton.states().size(), std::move(name));
    m_automaton.addState(s);
    std::vector<TransitionID> internalTransitions(internalDimensions);
    for (std::size_t i = 0; i < internalDimensions; i++) {
        this->addInternal(std::to_string(i), s.id());
    }
    return { s.id(), std::move(internalTransitions) };
}

TransitionID Bcifs::addBoundary(std::string name, StateID from, StateID to) {
    return this->addTransition(std::move(name), from, to, TransitionType::BOUNDARY);
}

TransitionID Bcifs::addSubdivision(std::string name, StateID from, StateID to) {
    return this->addTransition(std::move(name), from, to, TransitionType::SUBDIVISION);
}

TransitionID Bcifs::addPermutation(std::string name, StateID from, StateID to) {
    return this->addTransition(std::move(name), from, to, TransitionType::PERMUTATION);
}

TransitionID Bcifs::addInternal(std::string name, StateID stateID) {
    return this->addTransition(std::move(name), stateID, stateID, TransitionType::INTERNAL);
}

TransitionID Bcifs::addTransition(std::string name, StateID from, StateID to, TransitionType type) {
    Transition transition(m_automaton.transitions().size(), std::move(name), from, to, type);
    m_automaton.addTransition(transition);
    return transition.id();
}

void Bcifs::addConstraint(const Path& lhs, const Path& rhs) {
    if (m_automaton.isBoundaryOnly(lhs) && m_automaton.isBoundaryOnly(rhs)) {
        m_adjacencyConstraintsOnIncidenceOperators.emplace_back(lhs, rhs);
    } else {
        m_constraints.emplace_back(lhs, rhs);
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
    std::cout.flush();
}

void Bcifs::printConstraint(const std::pair<Path, Path>& constraint) const {
    std::cout << "[ ";
    bool firstTransition = true;
    for (const auto& transition: constraint.first) {
        Transition trans = m_automaton.findTransitionByID(transition);
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
        Transition trans = m_automaton.findTransitionByID(transition);
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

} // BCIFS