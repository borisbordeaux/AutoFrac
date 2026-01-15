#include "app/bcifs/bcifsbuilder.h"

namespace BCIFS {

BcifsBuilder::BcifsBuilder(Bcifs& bcifs, const std::string& filePath) : m_bcifs(bcifs) {
    m_bcifs.reset();
    m_mapStates.emplace("init", m_bcifs.addInitState());

    this->initializeLua();

    // open some common libraries
    m_lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::math);
    m_lua.safe_script_file(filePath);
}

void BcifsBuilder::state(const std::string& name, std::size_t internalDimension) {
    this->assertStateDoesntExist(name);
    auto [stateId, internalTransitionsIds] = m_bcifs.addState(name, internalDimension);
    m_mapStates.emplace(name, stateId);
    for (std::size_t internId = 0; internId < internalTransitionsIds.size(); internId++) {
        m_mapTransitions.emplace(name + "_intern_" + std::to_string(internId), internalTransitionsIds[internId]);
    }
}

void BcifsBuilder::boundary(const std::string& name, const std::string& from, const std::string& to) {
    this->assertTransitionDoesntExist(name);
    StateID fromId = this->getStateID(from);
    StateID toId = this->getStateID(to);
    m_mapTransitions.emplace(name, m_bcifs.addBoundary(std::move(name), fromId, toId));
}

void BcifsBuilder::grid(const std::string& state, const std::vector<std::vector<std::vector<std::string>>>& figures) {
    StateID stateId = this->getStateID(state);
    std::vector<Figure> trueFigures;
    for (const std::vector<std::vector<std::string>>& figure : figures) {
        Figure& currentFigure = trueFigures.emplace_back();
        for (const std::vector<std::string>& path : figure) {
            Path& currentPath = currentFigure.emplace_back();
            for (const std::string& transitionName : path) {
                TransitionID transitionId = this->getTransitionID(transitionName);
                currentPath.push_back(transitionId);
            }
        }
    }
    m_bcifs.addGrid(stateId, trueFigures);
}

void BcifsBuilder::subdivision(const std::string& name, const std::string& from, const std::string& to) {
    this->assertTransitionDoesntExist(name);
    StateID fromId = this->getStateID(from);
    StateID toId = this->getStateID(to);
    m_mapTransitions.emplace(name, m_bcifs.addSubdivision(std::move(name), fromId, toId));
}

void BcifsBuilder::permutation(const std::string& name, const std::string& from, const std::string& to) {
    this->assertTransitionDoesntExist(name);
    StateID fromId = this->getStateID(from);
    StateID toId = this->getStateID(to);
    m_mapTransitions.emplace(name, m_bcifs.addPermutation(std::move(name), fromId, toId));
}

void BcifsBuilder::space(const std::string& state, const std::vector<std::string>& boundaries) {
    StateID stateId = this->getStateID(state);
    std::vector<TransitionID> transitions;
    transitions.reserve(boundaries.size());
    for (const std::string& transitionName : boundaries) {
        TransitionID transitionId = this->getTransitionID(transitionName);
        transitions.push_back(transitionId);
    }
    m_bcifs.setSpace(stateId, transitions);
}

void BcifsBuilder::primitive(const std::string& state, const std::vector<std::vector<std::vector<std::string>>>& figures) {
    StateID stateId = this->getStateID(state);
    std::vector<Figure> trueFigures;
    for (const std::vector<std::vector<std::string>>& figure : figures) {
        Figure& currentFigure = trueFigures.emplace_back();
        for (const std::vector<std::string>& path : figure) {
            Path& currentPath = currentFigure.emplace_back();
            for (const std::string& transitionName : path) {
                TransitionID transitionId = this->getTransitionID(transitionName);
                currentPath.push_back(transitionId);
            }
        }
    }
    m_bcifs.setPrimitive(stateId, trueFigures);
}

void BcifsBuilder::constraint(const std::vector<std::string>& firstPath, const std::vector<std::string>& secondPath) {
    std::vector<TransitionID> first;
    for (const std::string& transitionName : firstPath) {
        TransitionID transitionId = this->getTransitionID(transitionName);
        first.push_back(transitionId);
    }
    std::vector<TransitionID> second;
    for (const std::string& transitionName : secondPath) {
        TransitionID transitionId = this->getTransitionID(transitionName);
        second.push_back(transitionId);
    }
    m_bcifs.addConstraint(first, second);
}

void BcifsBuilder::initMat(const std::string& transition, const std::vector<std::vector<float>>& matrix, const std::string& constness) {
    TransitionID transitionId = this->getTransitionID(transition);
    CoefType coefType = CoefType::CONST;
    if (constness == "VAR") {
        coefType = CoefType::VAR;
    } else if (constness != "CONST") {
        throw std::runtime_error("Constness must be \"VAR\" or \"CONST\"");
    }
    m_bcifs.setInitMat(transitionId, FormalMatrix(matrix, coefType));
}

void BcifsBuilder::initializeLua() {
    m_lua.set_function("state", [&](const std::string& name, std::size_t internalDimension) {
        this->state(name, internalDimension);
    });
    m_lua.set_function("boundary", [&](const std::string& name, const std::string& from, const std::string& to) {
        this->boundary(name, from, to);
    });
    //m_lua.set_function("grid", [&](const std::string& state, const & figures) {
    m_lua.set_function("grid", [&](const std::string& state, sol::as_table_t<std::vector<std::vector<std::vector<std::string>>>> figures) {
        this->grid(state, figures.value());
    });
    m_lua.set_function("subdivision", [&](const std::string& name, const std::string& from, const std::string& to) {
        this->subdivision(name, from, to);
    });
    m_lua.set_function("permutation", [&](const std::string& name, const std::string& from, const std::string& to) {
        this->permutation(name, from, to);
    });
    m_lua.set_function("space", [&](const std::string& state, sol::as_table_t<std::vector<std::string>> boundaries) {
        this->space(state, boundaries.value());
    });
    m_lua.set_function("primitive", [&](const std::string& state, sol::as_table_t<std::vector<std::vector<std::vector<std::string>>>> figures) {
        this->primitive(state, figures.value());
    });
    m_lua.set_function("constraint", [&](sol::as_table_t<std::vector<std::string>> firstPath, sol::as_table_t<std::vector<std::string>> secondPath) {
        this->constraint(firstPath.value(), secondPath.value());
    });
    m_lua.set_function("initMat", [&](const std::string& state, sol::as_table_t<std::vector<std::vector<float>>> matrix, const std::string& constness) {
        this->initMat(state, matrix.value(), constness);
    });
}

StateID BcifsBuilder::getStateID(const std::string& name) {
    auto it = m_mapStates.find(name);
    if (it == m_mapStates.end()) {
        throw std::runtime_error("State " + name + " does not exist");
    }
    return it->second;
}

TransitionID BcifsBuilder::getTransitionID(const std::string& name) {
    auto itTransition = m_mapTransitions.find(name);
    if (itTransition == m_mapTransitions.end()) {
        throw std::runtime_error("Transition " + name + " does not exist");
    }
    return itTransition->second;
}

void BcifsBuilder::assertStateDoesntExist(const std::string& name) {
    if (m_mapStates.find(name) != m_mapStates.end()) {
        throw std::runtime_error("State " + name + " already exists");
    }
}

void BcifsBuilder::assertTransitionDoesntExist(const std::string& name) {
    if (m_mapTransitions.find(name) != m_mapTransitions.end()) {
        throw std::runtime_error("Transition " + name + " already exists");
    }
}

} // BCIFS
