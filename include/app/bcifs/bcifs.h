#ifndef AUTOFRAC_BCIFS_H
#define AUTOFRAC_BCIFS_H

#include <glm/vec3.hpp>
#include <optional>
#include <string>
#include <unordered_map>

#include "automaton.h"
#include "formalmatrix.h"
#include "grid.h"
#include "massspringsystem.h"

namespace BCIFS {
class SubdivisionPoint;
class BcifsVertex;

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
    void addGrid(StateID id, Grid grid);
    void addGridFromBoundary(StateID id);
    void setSpace(StateID id, std::vector<TransitionID> transitions);
    void setPrimitive(StateID id, std::vector<Figure> primitive);
    void setPrimitiveMat(StateID id, const std::vector<FormalMatrix>& primitive);
    TransitionID addSubdivision(std::string name, StateID from, StateID to);
    TransitionID addSubdivision(std::string name, StateID from, StateID to, glm::vec3 frontColor);
    TransitionID addSubdivision(std::string name, StateID from, StateID to, glm::vec3 frontColor, glm::vec3 backColor);
    TransitionID addPermutation(std::string name, StateID from, StateID to);
    void addConstraint(const Path& lhs, const Path& rhs);
    void setInitMat(const Path& path, const FormalMatrix& matrix);
    std::string toString() const;
    void print() const;
    void check() const;
    void finalize();
    void reset();
    std::vector<std::vector<BcifsVertex>> faces(int iterationLevel);
    std::vector<FormalMatrix> controlPoints(std::size_t gridLevel) const;
    /**
     * Getter for all subdivision points.
     * @return a pair of vectors, the first one contains variable subdivision points and the second one contains constant subdivision points.
     */
    std::pair<std::vector<SubdivisionPoint>, std::vector<SubdivisionPoint>> subdivisionPoints(std::size_t gridLevel) const;
    std::vector<std::pair<glm::vec3, glm::vec3>> springs(std::size_t gridLevel) const;
    std::vector<std::pair<glm::vec3, glm::vec3>> controlPointsSprings(std::size_t gridLevel) const;
    void updateMSS();
    void updateMSSControlPoints();
    void printMSS() const;
    float* k() { return &m_k; }
    float* damping() { return &m_damping; }
    const Automaton& automaton() const { return m_automaton; }
    void invalidate(bool controlPointsOnly = false);
    void setColorDepth(std::size_t colorDepth);
    float* defaultFrontColor() { return &m_defaultFrontColor[0]; }
    float* defaultBackColor() { return &m_defaultBackColor[0]; }
    CoefPool* pool() { return &m_pool; }
    bool* cacheTransforms() { return &m_cacheTransforms; }
    bool* removeInternalFaces() { return &m_removeInternalFaces; }

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
    void checkPrimitives() const;
    void initializeMatrices(StateID id);
    void resolvePermutationConstraints(StateID id);
    void resolveConstraints();
    void initSubdivisionOperators();
    const FormalMatrix& getOrInitOperator(TransitionID id);
    const FormalMatrix& getOperator(TransitionID id) const;
    const arma::mat& getOperatorMat(TransitionID id) const;
    void printConstraintMatrices(const Constraint& constraint);
    void completeSubdivisionMatrices();
    void initMatCoefficients();
    void buildGridsFromBoundary();
    void buildMassSpringSystems();
    void buildMSSForControlPoints();
    arma::mat getOperatorOfPath(const Path& path) const;
    arma::mat getOperatorOfPathForPrimitive(const Path& path) const;
    arma::mat getOperatorOfPathForMSS(const Path& path) const;
    FormalMatrix getOperatorOfPathNoSubdivision(const Path& path) const;
    FormalMatrix globalMatrixOf(StateID id) const;
    const std::vector<arma::mat>& getPrimitiveMat(StateID id) const;
    void initPrimitives();
    const glm::vec3& getFrontColor(const Path& path) const;
    const glm::vec3& getBackColor(const Path& path) const;
    bool needToHideFace(std::size_t index, const Path& path) const;
    bool needToHideBoundary(TransitionID subdivisionId, TransitionID boundaryId) const;
    std::optional<TransitionID> findFirstTransitionOfPrimitivePath(std::size_t indexFace, StateID id) const;
    std::optional<Constraint> findIncidenceConstraintOf(TransitionID subdivisionId, TransitionID boundaryId) const;

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
    std::unordered_map<StateID, Grid> m_mapGrids;
    std::vector<StateID> m_createGridFromBoundary;
    std::unordered_map<StateID, MassSpringSystem> m_mapMSS;
    MassSpringSystem m_MSSControlPoints;
    float m_damping = 0.3f;
    float m_k = 0.005f;
    float m_dampingControlPoints = 0.3f;
    float m_kControlPoints = 0.02f;
    float m_lengthControlPoints = 1.0f;
    std::vector<std::pair<Path, FormalMatrix>> m_initMatrices;
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
    CoefPool m_pool;
    bool m_cacheTransforms = true;
    bool m_removeInternalFaces = true;
};

} // BCIFS

#endif //AUTOFRAC_BCIFS_H
