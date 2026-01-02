#ifndef AUTOFRAC_BCIFS_H
#define AUTOFRAC_BCIFS_H

#include <string>
#include <unordered_map>
#include <glm/vec3.hpp>
#include "app/automaton/automaton.h"
#include "formalmatrix.h"
#include "app/massspringsystem/massspringsystem.h"

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
    StateID addInitState();
    TransitionID addBoundary(std::string name, StateID from, StateID to);
    void addGrid(StateID id, std::vector<Figure> grid);
    void setSpace(StateID id, std::vector<TransitionID> transitions);
    TransitionID addSubdivision(std::string name, StateID from, StateID to);
    TransitionID addPermutation(std::string name, StateID from, StateID to);
    void addConstraint(const Path& lhs, const Path& rhs);
    void setInitMat(TransitionID id, const FormalMatrix& matrix);
    std::string toString() const;
    void print() const;
    void validate();

    void reset();
    std::vector<std::vector<glm::vec3>> faces(int iterationLevel);
    std::vector<FormalMatrix> controlPoints() const;
    std::vector<std::pair<glm::vec3, glm::vec3>> springs() const;
    void updateMSS();
    void printMSS() const;

    inline float* k() { return &m_k; }

    inline float* damping() { return &m_damping; }

    void invalidate();
    Automaton& automaton() { return m_automaton; }
    StateID initState() const { return m_initStateID.value_or(-1); }

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

    std::string printConstraint(const Constraint& constraint) const;

    void checkAutomaton() const;
    void checkSpaces() const;
    void checkConstraints() const;
    void initializeMatrices();
    void initializeMatrices(StateID id);

    void resolvePermutationConstraints(StateID id);
    void resolveConstraints();
    void initSubdivisionOperators();
    const FormalMatrix& getOrInitOperator(TransitionID id);
    const FormalMatrix& getOperator(TransitionID id) const;

    void printConstraintMatrices(const Constraint& constraint);

    void completeSubdivisionMatrices();

    void buildMassSpringSystems();

    FormalMatrix getOperatorOfPath(const Path& path) const;
    FormalMatrix getOperatorOfPathForMSS(const Path& path) const;
    FormalMatrix getOperatorOfPathNoSubdivision(const Path& path) const;
    FormalMatrix globalMatrixOf(StateID id) const;

private:
    Automaton m_automaton;
    std::optional<StateID> m_initStateID;
    std::unordered_map<StateID, std::vector<TransitionID>> m_mapSpaces;
    std::vector<Constraint> m_constraints;
    std::vector<Constraint> m_adjacencyConstraintsOnIncidenceOperators;
    std::vector<Constraint> m_permutationConstraints;
    std::unordered_map<StateID, std::size_t> m_mapDimensions;
    std::unordered_map<TransitionID, FormalMatrix> m_mapOperators;
    std::unordered_map<StateID, std::vector<Figure>> m_mapGrids;
    std::unordered_map<StateID, mss::MassSpringSystem> m_mapMSS;
    float m_damping = 0.3f;
    float m_k = 0.05f;
    std::unordered_map<TransitionID, FormalMatrix> m_mapInitMat;
    bool m_invalidatedMatrices = true;
    std::vector<std::pair<Path, FormalMatrix>> m_facesPaths;
};

} // BCIFS

#endif //AUTOFRAC_BCIFS_H
