#ifndef AUTOFRAC_AUTOMATON_H
#define AUTOFRAC_AUTOMATON_H

#include <vector>
#include <optional>
#include "state.h"
#include "transition.h"

namespace BCIFS {

class Automaton {
public:
    Automaton() = default;

    void addState(const State& state);
    void addTransition(const Transition& transition);

    inline const std::vector<State>& states() const { return m_states; }

    inline const std::vector<Transition>& transitions() const { return m_transitions; }

    bool containsSubdvision(const Path& path) const;

    const Transition& findTransitionByID(TransitionID id) const;
    const State& findStateByID(StateID id) const;

    void print() const;

    std::size_t internalDimensions(StateID id) const;

    /**
     * Throws an exception if the check fails
     */
    void check() const;

    std::vector<TransitionID> boundaryTransitionsOf(StateID id) const;
    std::vector<StateID> boundaryStatesOf(StateID id) const;
    std::vector<TransitionID> internalTransitionsOf(StateID id) const;
    std::vector<TransitionID> boundaryAndInternalTransitionsOf(StateID id) const;

    void reset();

private:
    std::vector<State> m_states;
    std::vector<Transition> m_transitions;
};

} // BCIFS

#endif //AUTOFRAC_AUTOMATON_H
