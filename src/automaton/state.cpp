#include <iostream>
#include "automaton/state.h"

namespace BCIFS {

State::State(StateID id, std::string name) : m_id(id), m_name(std::move(name)) {}

} // BCIFS