#include "app/bcifs/transition.h"

#include "core/log.h"

#include <iostream>
#include <utility>

namespace BCIFS {

Transition::Transition(TransitionID id, std::string name, StateID from, StateID to, TransitionType type) :
    m_id(id), m_name(std::move(name)), m_from(from), m_to(to), m_type(type) {}

std::string Transition::toString() const {
    std::string res;
    switch (m_type) {
        case TransitionType::INTERNAL:
            res = "Internal(";
            break;
        case TransitionType::BOUNDARY:
            res = "Boundary(";
            break;
        case TransitionType::SUBDIVISION:
            res = "Subdivision(";
            break;
        case TransitionType::PERMUTATION:
            res = "Permutation(";
            break;
    }
    res += m_name + ")";
    return res;
}

void Transition::print() const {
    Core::LOG_INFO(this->toString());
}

} // BCIFS
