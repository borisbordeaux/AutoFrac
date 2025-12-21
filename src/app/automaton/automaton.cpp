#include "app/automaton/automaton.h"
#include "app/automaton/transition.h"
#include <algorithm>
#include <stdexcept>
#include <iostream>

namespace BCIFS {

void Automaton::addState(const State& state) {
    m_states.push_back(state);
}

void Automaton::addTransition(const Transition& transition) {
    m_transitions.push_back(transition);
}

bool Automaton::containsSubdvision(const Path& path) const {
    return std::any_of(path.begin(), path.end(), [this](TransitionID id) {
        const Transition& transition = this->findTransitionByID(id);
        return transition.type() == TransitionType::SUBDIVISION;
    });
}

const Transition& Automaton::findTransitionByID(TransitionID id) const {
    for (const Transition& transition: m_transitions) {
        if (transition.id() == id) {
            return transition;
        }
    }

    throw std::runtime_error("Transition not found");
}

const State& Automaton::findStateByID(StateID id) const {
    for (const State& state: m_states) {
        if (state.id() == id) {
            return state;
        }
    }

    throw std::runtime_error("State not found");
}

void Automaton::print() const {
    std::cout << "States:\n";
    for (const State& state: m_states) {
        std::cout << state.name() << " with " << this->internalDimensions(state.id()) << " internal dimensions\n";
    }
    std::cout << "\nTransitions:\n";
    for (const Transition& transition: m_transitions) {
        std::cout << this->findStateByID(transition.from()).name() << " ";
        transition.print();
        std::cout << " " << this->findStateByID(transition.to()).name() << "\n";
    }
    std::cout << std::endl;
}

std::size_t Automaton::internalDimensions(StateID id) const {
    std::size_t res = 0;
    for (const Transition& transition: m_transitions) {
        if (transition.type() == TransitionType::INTERNAL && transition.from() == id) {
            res++;
        }
    }
    return res;
}

void Automaton::check() const {
    for (const Transition& transition: m_transitions) {
        this->findStateByID(transition.from());
        this->findStateByID(transition.to());
    }
}

std::vector<TransitionID> Automaton::boundaryTransitionsOf(StateID id) const {
    std::vector<TransitionID> res;
    for (const Transition& transition: m_transitions) {
        if (transition.from() == id && transition.type() == TransitionType::BOUNDARY) {
            res.push_back(transition.id());
        }
    }
    return res;
}

std::vector<StateID> Automaton::boundaryStatesOf(StateID id) const {
    std::vector<StateID> res;
    for (const Transition& transition: m_transitions) {
        if (transition.from() == id && transition.type() == TransitionType::BOUNDARY) {
            res.push_back(transition.to());
        }
    }
    return res;
}

std::vector<TransitionID> Automaton::internalTransitionsOf(StateID id) const {
    std::vector<TransitionID> res;
    for (const Transition& transition: m_transitions) {
        if (transition.from() == id && transition.type() == TransitionType::INTERNAL) {
            res.push_back(transition.id());
        }
    }
    return res;
}

std::vector<TransitionID> Automaton::boundaryAndInternalTransitionsOf(StateID id) const {
    std::vector<TransitionID> res;
    for (const Transition& transition: m_transitions) {
        if (transition.from() == id && (transition.type() == TransitionType::BOUNDARY || transition.type() == TransitionType::INTERNAL)) {
            res.push_back(transition.id());
        }
    }
    return res;
}

void Automaton::reset() {
    m_states.clear();
    m_transitions.clear();
}

std::vector<TransitionID> Automaton::subdivisionTransitionsOf(StateID id) const {
    std::vector<TransitionID> res;
    for (const Transition& transition: m_transitions) {
        if (transition.from() == id && transition.type() == TransitionType::SUBDIVISION) {
            res.push_back(transition.id());
        }
    }
    return res;
}

std::vector<Path> Automaton::allSubdivisionPaths(StateID from, int depth) const {
    std::vector<Path> res;
    Path currentPath;

    this->dfs(from, 0, depth, currentPath, res);

    return res;
}

void Automaton::dfs(StateID from, int depth, int maxDepth, Path& currentPath, std::vector<Path>& result) const {
    if (depth == maxDepth) {
        result.push_back(currentPath); // copie
        return;
    }

    for (TransitionID transitionId : this->subdivisionTransitionsOf(from)) {
        currentPath.push_back(transitionId);
        const Transition& t = findTransitionByID(transitionId);
        this->dfs(t.to(), depth + 1, maxDepth, currentPath, result);
        currentPath.pop_back();
    }
}

} // BCIFS