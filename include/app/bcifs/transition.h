#ifndef AUTOFRAC_TRANSITION_H
#define AUTOFRAC_TRANSITION_H

#include <string>
#include <vector>

#include "state.h"

namespace BCIFS {

enum class TransitionType {
    INTERNAL, BOUNDARY, SUBDIVISION, PERMUTATION
};

using TransitionID = std::size_t;
using Path = std::vector<TransitionID>;
using Figure = std::vector<Path>;

class Transition {
public:
    Transition(TransitionID id, std::string name, StateID from, StateID to, TransitionType type);
    TransitionID id() const { return m_id; }
    const std::string& name() const { return m_name; }
    StateID from() const { return m_from; }
    StateID to() const { return m_to; }
    TransitionType type() const { return m_type; }
    std::string toString() const;
    void print() const;

private:
    TransitionID m_id;
    std::string m_name;
    StateID m_from;
    StateID m_to;
    TransitionType m_type;
};

} // BCIFS

#endif //AUTOFRAC_TRANSITION_H
