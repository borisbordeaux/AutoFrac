#ifndef AUTOFRAC_BCIFS_H
#define AUTOFRAC_BCIFS_H

#include <string>
#include <unordered_map>
#include <glm/vec3.hpp>
#include "app/automaton/automaton.h"
#include "formalmatrix.h"
#include "app/massspringsystem/massspringsystem.h"

namespace BCIFS {

class SubdivisionPoint {
public:
    SubdivisionPoint(FormalMatrix T, FormalMatrix posBary);
    const FormalMatrix& T() const { return m_T; }
    const FormalMatrix& posBary() const { return m_posBary; }
    FormalMatrix& posBary() { return m_posBary; }
    glm::vec3 posR3() const;

private:
    FormalMatrix m_T;
    FormalMatrix m_posBary;
};

class BcifsPoint {
public:
    BcifsPoint(glm::vec3 pos, glm::vec3 frontColor, glm::vec3 backColor);
    const glm::vec3& pos() const { return m_pos; }
    const glm::vec3& frontColor() const { return m_frontColor; }
    const glm::vec3& backColor() const { return m_backColor; }

private:
    glm::vec3 m_pos;
    glm::vec3 m_frontColor;
    glm::vec3 m_backColor;
};

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
    void setPrimitive(StateID id, std::vector<Figure> primitive);
    TransitionID addSubdivision(std::string name, StateID from, StateID to);
    TransitionID addSubdivision(std::string name, StateID from, StateID to, glm::vec3 frontColor);
    TransitionID addSubdivision(std::string name, StateID from, StateID to, glm::vec3 frontColor, glm::vec3 backColor);
    TransitionID addPermutation(std::string name, StateID from, StateID to);
    void addConstraint(const Path& lhs, const Path& rhs);
    void setInitMat(TransitionID id, const FormalMatrix& matrix);
    std::string toString() const;
    void print() const;
    void check() const;
    void finalize();

    void reset();
    std::vector<std::vector<BcifsPoint>> faces(int iterationLevel);
    std::vector<FormalMatrix> controlPoints() const;
    /**
     * Getter for all subdivision points.
     * @return a pair of vectors, the first one contains variable subdivision points and the second one contains constant subdivision points.
     */
    std::pair<std::vector<SubdivisionPoint>, std::vector<SubdivisionPoint>> subdivisionPoints() const;
    std::vector<std::pair<glm::vec3, glm::vec3>> springs() const;
    std::vector<std::pair<glm::vec3, glm::vec3>> controlPointsSprings() const;
    void updateMSS();
    void updateMSSControlPoints();
    void printMSS() const;

    inline float* k() { return &m_k; }

    inline float* damping() { return &m_damping; }

    void invalidate(bool controlPointsOnly = false);

    const Automaton& automaton() const { return m_automaton; }

    void setColorDepth(std::size_t colorDepth);
    float* defaultFrontColor() { return &m_defaultFrontColor[0]; }
    float* defaultBackColor() { return &m_defaultBackColor[0]; }

private:
    using Constraint = std::pair<Path, Path>;

    enum class ConstraintType {
        SUBDIVISION,                     // constrains subdivision operators
        PERMUTATION,                     // constrains permutation operators
        ADJACENCY_ON_INCIDENCE_OPERATORS // merge spaces
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
    const arma::mat& getOperatorMat(TransitionID id) const;
    void printConstraintMatrices(const Constraint& constraint);
    void completeSubdivisionMatrices();
    void buildMassSpringSystems();
    void buildMSSForControlPoints();
    arma::mat getOperatorOfPath(const Path& path) const;
    arma::mat getOperatorOfPathForPrimitive(const Path& path) const;
    FormalMatrix getOperatorOfPathForMSS(const Path& path) const;
    FormalMatrix getOperatorOfPathNoSubdivision(const Path& path) const;
    FormalMatrix globalMatrixOf(StateID id) const;
    const std::vector<arma::mat>& getPrimitiveMat(StateID id) const;
    void initPrimitives();
    const glm::vec3& getFrontColor(const Path& path) const;
    const glm::vec3& getBackColor(const Path& path) const;

private:
    Automaton m_automaton;
    std::optional<StateID> m_initStateID;
    std::unordered_map<StateID, std::vector<TransitionID>> m_mapSpaces;
    std::vector<Constraint> m_constraints;
    std::vector<Constraint> m_adjacencyConstraintsOnIncidenceOperators;
    std::vector<Constraint> m_permutationConstraints;
    std::unordered_map<StateID, std::size_t> m_mapDimensions;
    std::unordered_map<TransitionID, FormalMatrix> m_mapOperators;
    std::unordered_map<TransitionID, arma::mat> m_mapOperatorsMat;
    std::unordered_map<StateID, std::vector<Figure>> m_mapGrids;
    std::unordered_map<StateID, mss::MassSpringSystem> m_mapMSS;
    mss::MassSpringSystem m_MSSControlPoints;
    float m_damping = 0.3f;
    float m_k = 0.005f;
    float m_dampingControlPoints = 0.3f;
    float m_kControlPoints = 0.02f;
    float m_lengthControlPoints = 1.0f;
    std::unordered_map<TransitionID, FormalMatrix> m_mapInitMat;
    bool m_invalidatedMatrices = true;
    bool m_invalidatedMatricesControlPoints = false;
    std::vector<std::pair<Path, arma::mat>> m_facesPaths;
    std::unordered_map<StateID, std::vector<Figure>> m_mapPrimitives;
    std::unordered_map<StateID, std::vector<arma::mat>> m_mapPrimitivesMat;
    bool m_needUpdatePrimitivesWhenChangingMatrices = true;
    glm::vec3 m_defaultFrontColor = glm::vec3(100.0f / 255.0f, 100.0f / 255.0f, 100.0f / 255.0f);
    glm::vec3 m_defaultBackColor = glm::vec3(50.0f / 255.0f, 50.0f / 255.0f, 50.0f / 255.0f);
    std::unordered_map<TransitionID, glm::vec3> m_frontColors;
    std::unordered_map<TransitionID, glm::vec3> m_backColors;
    std::size_t m_colorDepth = 0;
};

} // BCIFS

#endif //AUTOFRAC_BCIFS_H
