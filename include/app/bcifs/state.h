#ifndef AUTOFRAC_STATE_H
#define AUTOFRAC_STATE_H

#include <string>

namespace BCIFS {

using StateID = std::size_t;

class State {
public:
    State(StateID id, std::string name);
    const std::string& name() const { return m_name; }
    StateID id() const { return m_id; }

private:
    StateID m_id;
    std::string m_name;
};

} // BCIFS

#endif //AUTOFRAC_STATE_H
