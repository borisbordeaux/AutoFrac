#include "automaton/transition.h"

#include <utility>
#include <iostream>

namespace BCIFS {

Transition::Transition(TransitionID id, std::string name, StateID from, StateID to, TransitionType type) :
        m_id(id), m_name(std::move(name)), m_from(from), m_to(to), m_type(type) {}

void Transition::print() const {
    switch (m_type) {
        case TransitionType::INTERNAL:
            std::cout << "Internal(";
            break;
        case TransitionType::BOUNDARY:
            std::cout << "Boundary(";
            break;
        case TransitionType::SUBDIVISION:
            std::cout << "Subdivision(";
            break;
        case TransitionType::PERMUTATION:
            std::cout << "Permutation(";
            break;
    }
    std::cout << m_name << ")";
}

} // BCIFS