#include "automaton/automaton.h"
#include "automaton/transition.h"
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

std::vector<TransitionID> Automaton::boundaryTransitionOf(StateID id) const {
    std::vector<TransitionID> res;
    for (const Transition& transition: m_transitions) {
        if (transition.from() == id && transition.type() == TransitionType::BOUNDARY) {
            res.push_back(transition.id());
        }
    }
    return res;
}

std::vector<StateID> Automaton::boundaryStateOf(StateID id) const {
    std::vector<StateID> res;
    for (const Transition& transition: m_transitions) {
        if (transition.from() == id && transition.type() == TransitionType::BOUNDARY) {
            res.push_back(transition.to());
        }
    }
    return res;
}

std::vector<TransitionID> Automaton::internalTransitionOf(StateID id) const {
    std::vector<TransitionID> res;
    for (const Transition& transition: m_transitions) {
        if (transition.from() == id && transition.type() == TransitionType::INTERNAL) {
            res.push_back(transition.id());
        }
    }
    return res;
}

std::vector<TransitionID> Automaton::boundaryAndInternalTransitionOf(StateID id) const {
    std::vector<TransitionID> res;
    for (const Transition& transition: m_transitions) {
        if (transition.from() == id && (transition.type() == TransitionType::BOUNDARY || transition.type() == TransitionType::INTERNAL)) {
            res.push_back(transition.id());
        }
    }
    return res;
}

} // BCIFS