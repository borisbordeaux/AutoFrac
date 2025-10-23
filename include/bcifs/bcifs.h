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
    void setSpace(StateID id, std::vector<TransitionID> transitions);
    TransitionID addSubdivision(std::string name, StateID from, StateID to);
    TransitionID addPermutation(std::string name, StateID from, StateID to);
    void addConstraint(const Path& lhs, const Path& rhs);
    void print() const;
    void validate();

private:
    using Constraint = std::pair<Path, Path>;
    enum class ConstraintType {
        SUBDIVISION,  // constrains subdivision operators
        PERMUTATION,  // constrains permutation operators
        ADJACENCY_ON_INCIDENCE_OPERATORS  // merge spaces
    };

    ConstraintType constraintType(const Constraint& constraint) const;
    bool isSubdivisionConstraint(const Constraint& constraint) const;

    TransitionID addInternal(std::string name, StateID stateID);
    TransitionID addTransition(std::string name, StateID from, StateID to, TransitionType type);

    void printConstraint(const Constraint& constraint) const;

    void checkAutomaton() const;
    void checkSpaces() const;
    void checkConstraints() const;
    void initializeMatrices();
    void initializeMatrices(StateID id);

    void resolvePermutationConstraints(StateID id);
    void resolveConstraints();
    const FormalMatrix& getOrInitOperator(TransitionID id);

    void printConstraintMatrices(const Constraint& constraint);

    void completeSubdvisionMatrices();

private:
    Automaton m_automaton;
    std::unordered_map<StateID, std::vector<TransitionID>> m_mapSpaces;
    std::vector<Constraint> m_constraints;
    std::vector<Constraint> m_adjacencyConstraintsOnIncidenceOperators;
    std::vector<Constraint> m_permutationConstraints;
    std::unordered_map<StateID, std::size_t> m_mapDimensions;
    std::unordered_map<TransitionID, FormalMatrix> m_mapOperators;
};

} // BCIFS

#endif //AUTOFRAC_BCIFS_H
