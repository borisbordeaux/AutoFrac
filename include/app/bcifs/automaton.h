#ifndef AUTOFRAC_AUTOMATON_H
#define AUTOFRAC_AUTOMATON_H

#include <unordered_map>
#include <vector>

#include "state.h"
#include "transition.h"

namespace BCIFS {

class Automaton {
public:
    Automaton() = default;
    StateID addState(std::string name);
    void addTransition(const Transition& transition);
    const std::vector<State>& states() const { return m_states; }
    const std::vector<Transition>& transitions() const { return m_transitions; }
    bool containsSubdivision(const Path& path) const;
    const Transition& findTransitionByID(TransitionID id) const;
    const State& findStateByID(StateID id) const;
    std::string toString() const;
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
    std::vector<TransitionID> subdivisionTransitionsOf(StateID id) const;
    std::vector<TransitionID> subdivisionTransitionsTo(StateID id, StateID ignoredId) const;
    std::vector<Path> allSubdivisionPaths(StateID from, int depth) const;
    std::unordered_map<StateID, Path> shortestPaths(StateID from) const;
    void reset();

private:
    void dfs(StateID from, int depth, int maxDepth, Path& currentPath, std::vector<Path>& result) const;

private:
    std::vector<State> m_states;
    std::vector<Transition> m_transitions;
};

} // BCIFS

#endif //AUTOFRAC_AUTOMATON_H
