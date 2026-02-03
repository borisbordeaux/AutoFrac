#include "app/bcifs/state.h"

#include <iostream>

namespace BCIFS {

State::State(StateID id, std::string name) : m_id(id), m_name(std::move(name)) {}

} // BCIFS
