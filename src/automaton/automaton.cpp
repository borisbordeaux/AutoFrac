#include <algorithm>
#include <stdexcept>
#include <iostream>
#include "automaton/automaton.h"
#include "automaton/transition.h"

namespace BCIFS {

void Automaton::addState(const State& state) {
    m_states.push_back(state);
}

void Automaton::addTransition(const Transition& transition) {
    m_transitions.push_back(transition);
}

bool Automaton::isBoundaryOnly(const Path& path) const {
    return std::all_of(path.begin(), path.end(), [this](TransitionID id) {
        const Transition& transition = this->findTransitionByID(id);
        return transition.type() == TransitionType::BOUNDARY;
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

} // BCIFS