#ifndef AUTOFRAC_BCIFSBUILDER_H
#define AUTOFRAC_BCIFSBUILDER_H

#include <string>
#include <unordered_map>
#include <vector>

#include "bcifs.h"
#include "sol2/sol.h"

namespace BCIFS {

using PathName = std::vector<std::string>;

using FigureName = std::vector<PathName>;

class GridFigureName {
public:
    explicit GridFigureName(FigureName paths, float k = -1.0f, float length = -1.0f);
    const FigureName& paths() const { return m_paths; }
    float k() const { return m_k; }
    float length() const { return m_length; }

private:
    FigureName m_paths;
    float m_k;
    float m_length;
};

using GridName = std::vector<GridFigureName>;

class BcifsBuilder {
public:
    explicit BcifsBuilder(Bcifs& bcifs, const std::string& filePath);
    void state(const std::string& name, std::size_t internalDimension);
    void boundary(const std::string& name, const std::string& from, const std::string& to);
    void grid(const std::string& state, const GridName& grid);
    void gridFromBoundary(const std::string& state);
    void subdivision(const std::string& name, const std::string& from, const std::string& to);
    void subdivision(const std::string& name, const std::string& from, const std::string& to, const std::vector<float>& frontColor);
    void subdivision(const std::string& name, const std::string& from, const std::string& to, const std::vector<float>& frontColor, const std::vector<float>& backColor);
    void permutation(const std::string& name, const std::string& from, const std::string& to);
    void space(const std::string& state, const std::vector<std::string>& boundaries);
    void primitive(const std::string& state, const std::vector<std::vector<std::vector<std::string>>>& figures);
    void constraint(const std::string& state, const std::vector<std::string>& firstPath, const std::vector<std::string>& secondPath);
    void initMat(const std::string& state, const std::string& transition, const std::vector<std::vector<float>>& matrix, const std::string& constness);

private:
    void initializeLua();
    static std::vector<float> parseColor(sol::object obj);
    static GridFigureName parseFigureName(const sol::table& figTable);
    StateID getStateID(const std::string& name);
    TransitionID getTransitionID(StateID stateId, const std::string& name);
    void assertStateDoesntExist(const std::string& name);
    void assertTransitionDoesntExist(StateID stateId, const std::string& name);

private:
    sol::state m_lua;

    std::unordered_map<std::string, StateID> m_mapStates;
    std::unordered_map<StateID, std::unordered_map<std::string, TransitionID>> m_mapTransitions;
    Bcifs& m_bcifs;
};

} // BCIFS

#endif //AUTOFRAC_BCIFSBUILDER_H
