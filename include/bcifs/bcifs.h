#ifndef AUTOFRAC_BCIFS_H
#define AUTOFRAC_BCIFS_H

#include <string>
#include <unordered_map>
#include "automaton/automaton.h"
#include "formalmatrix.h"

namespace BCIFS {

class Bcifs {
public:
    Bcifs() = default;

    /**
     * Create and add a state to the automaton
     * @param name the name of the state
     * @param internalDimensions the number of internal dimensions (i.e. unaccessible dimensions with boundary operators)
     * @return a pair containing the state ID and all its internal transition ID, one for each given intern dimension
     */
    std::pair<StateID, std::vector<TransitionID>> addState(std::string name, std::size_t internalDimensions);
    TransitionID addBoundary(std::string name, StateID from, StateID to);
    TransitionID addSubdivision(std::string name, StateID from, StateID to);
    TransitionID addPermutation(std::string name, StateID from, StateID to);
    void addConstraint(const Path& lhs, const Path& rhs);
    void print() const;

private:
    TransitionID addInternal(std::string name, StateID stateID);
    TransitionID addTransition(std::string name, StateID from, StateID to, TransitionType type);

    void printConstraint(const std::pair<Path, Path>& constraint) const;

private:
    Automaton m_automaton;
    std::vector<std::pair<Path, Path>> m_constraints;
    std::vector<std::pair<Path, Path>> m_adjacencyConstraintsOnIncidenceOperators;
};

} // BCIFS

#endif //AUTOFRAC_BCIFS_H
