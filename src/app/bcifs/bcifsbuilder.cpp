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
        m_mapTransitions[stateId].emplace("intern_" + std::to_string(internId), internalTransitionsIds[internId]);
    }
}

void BcifsBuilder::boundary(const std::string& name, const std::string& from, const std::string& to) {
    StateID fromId = this->getStateID(from);
    this->assertTransitionDoesntExist(fromId, name);
    StateID toId = this->getStateID(to);
    m_mapTransitions[fromId].emplace(name, m_bcifs.addBoundary(std::move(name), fromId, toId));
}

void BcifsBuilder::grid(const std::string& state, const std::vector<std::vector<std::vector<std::string>>>& figures) {
    StateID stateId = this->getStateID(state);
    std::vector<Figure> trueFigures;
    for (const std::vector<std::vector<std::string>>& figure : figures) {
        Figure& currentFigure = trueFigures.emplace_back();
        for (const std::vector<std::string>& path : figure) {
            Path& currentPath = currentFigure.emplace_back();
            StateID currentStateId = stateId;
            for (const std::string& transitionName : path) {
                TransitionID transitionId = this->getTransitionID(currentStateId, transitionName);
                const Transition& transition = m_bcifs.automaton().findTransitionByID(transitionId);
                currentStateId = transition.to();
                currentPath.push_back(transitionId);
            }
        }
    }
    m_bcifs.addGrid(stateId, trueFigures);
}

void BcifsBuilder::gridFromBoundary(const std::string& state) {
    StateID stateId = this->getStateID(state);
    m_bcifs.addGridFromBoundary(stateId);
}

void BcifsBuilder::subdivision(const std::string& name, const std::string& from, const std::string& to) {
    StateID fromId = this->getStateID(from);
    this->assertTransitionDoesntExist(fromId, name);
    StateID toId = this->getStateID(to);
    m_mapTransitions[fromId].emplace(name, m_bcifs.addSubdivision(std::move(name), fromId, toId));
}

void BcifsBuilder::subdivision(const std::string& name, const std::string& from, const std::string& to, const std::vector<float>& frontColor) {
    StateID fromId = this->getStateID(from);
    this->assertTransitionDoesntExist(fromId, name);
    StateID toId = this->getStateID(to);
    if (frontColor.size() != 3) {
        throw sol::error("The color must be an array of 3 floats");
    }
    glm::vec3 frontColorVec(frontColor[0], frontColor[1], frontColor[2]);
    m_mapTransitions[fromId].emplace(name, m_bcifs.addSubdivision(std::move(name), fromId, toId, std::move(frontColorVec)));
}

void BcifsBuilder::subdivision(const std::string& name, const std::string& from, const std::string& to, const std::vector<float>& frontColor, const std::vector<float>& backColor) {
    StateID fromId = this->getStateID(from);
    this->assertTransitionDoesntExist(fromId, name);
    StateID toId = this->getStateID(to);
    if (frontColor.size() != 3 || backColor.size() != 3) {
        throw sol::error("The color must be an array of 3 floats");
    }
    glm::vec3 frontColorVec(frontColor[0], frontColor[1], frontColor[2]);
    glm::vec3 backColorVec(backColor[0], backColor[1], backColor[2]);
    m_mapTransitions[fromId].emplace(name, m_bcifs.addSubdivision(std::move(name), fromId, toId, std::move(frontColorVec), std::move(backColorVec)));
}

void BcifsBuilder::permutation(const std::string& name, const std::string& from, const std::string& to) {
    StateID fromId = this->getStateID(from);
    this->assertTransitionDoesntExist(fromId, name);
    StateID toId = this->getStateID(to);
    m_mapTransitions[fromId].emplace(name, m_bcifs.addPermutation(std::move(name), fromId, toId));
}

void BcifsBuilder::space(const std::string& state, const std::vector<std::string>& boundaries) {
    StateID stateId = this->getStateID(state);
    std::vector<TransitionID> transitions;
    transitions.reserve(boundaries.size());
    for (const std::string& transitionName : boundaries) {
        TransitionID transitionId = this->getTransitionID(stateId, transitionName);
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
            StateID currentStateId = stateId;
            for (const std::string& transitionName : path) {
                TransitionID transitionId = this->getTransitionID(currentStateId, transitionName);
                const Transition& transition = m_bcifs.automaton().findTransitionByID(transitionId);
                currentStateId = transition.to();
                currentPath.push_back(transitionId);
            }
        }
    }
    m_bcifs.setPrimitive(stateId, trueFigures);
}

void BcifsBuilder::constraint(const std::string& state, const std::vector<std::string>& firstPath, const std::vector<std::string>& secondPath) {
    StateID stateId = this->getStateID(state);
    std::vector<TransitionID> first;
    StateID currentStateId = stateId;
    for (const std::string& transitionName : firstPath) {
        TransitionID transitionId = this->getTransitionID(currentStateId, transitionName);
        const Transition& transition = m_bcifs.automaton().findTransitionByID(transitionId);
        currentStateId = transition.to();
        first.push_back(transitionId);
    }
    std::vector<TransitionID> second;
    currentStateId = stateId;
    for (const std::string& transitionName : secondPath) {
        TransitionID transitionId = this->getTransitionID(currentStateId, transitionName);
        const Transition& transition = m_bcifs.automaton().findTransitionByID(transitionId);
        currentStateId = transition.to();
        second.push_back(transitionId);
    }
    m_bcifs.addConstraint(first, second);
}

void BcifsBuilder::initMat(const std::string& state, const std::string& transition, const std::vector<std::vector<float>>& matrix, const std::string& constness) {
    StateID stateId = this->getStateID(state);
    TransitionID transitionId = this->getTransitionID(stateId, transition);
    CoefKind coefType = CoefKind::CONST;
    if (constness == "VAR") {
        coefType = CoefKind::VAR;
    } else if (constness != "CONST") {
        throw sol::error("Constness must be \"VAR\" or \"CONST\"");
    }
    m_bcifs.setInitMat(transitionId, FormalMatrix(matrix, coefType, m_bcifs.pool()));
}

void BcifsBuilder::initializeLua() {
    m_lua.set_function("state", [&](const std::string& name, std::size_t internalDimension) {
        this->state(name, internalDimension);
    });
    m_lua.set_function("boundary", [&](const std::string& name, const std::string& from, const std::string& to) {
        this->boundary(name, from, to);
    });
    m_lua.set_function("grid", [&](const std::string& state, sol::as_table_t<std::vector<std::vector<std::vector<std::string>>>> figures) {
        this->grid(state, figures.value());
    });
    m_lua.set_function("gridFromBoundary", [&](const std::string& state) {
        this->gridFromBoundary(state);
    });
    m_lua.set_function("subdivision", [&](const sol::variadic_args& args) {
        if (args.size() != 3 && args.size() != 4 && args.size() != 5) {
            throw sol::error("subdivision(): expected (name, from, to [, frontColor[, backColor]])");
        }
        std::string name = args[0].as<std::string>();
        std::string from = args[1].as<std::string>();
        std::string to = args[2].as<std::string>();
        if (args.size() == 3) {
            this->subdivision(name, from, to);
            return;
        }
        std::vector<float> frontColor = BcifsBuilder::parseColor(args[3]);
        if (args.size() == 4) {
            this->subdivision(name, from, to, frontColor);
            return;
        }
        std::vector<float> backColor = BcifsBuilder::parseColor(args[4]);
        this->subdivision(name, from, to, frontColor, backColor);
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
    m_lua.set_function("constraint", [&](const std::string& state, sol::as_table_t<std::vector<std::string>> firstPath, sol::as_table_t<std::vector<std::string>> secondPath) {
        this->constraint(state, firstPath.value(), secondPath.value());
    });
    m_lua.set_function("initMat", [&](const std::string& state, const std::string& transition, sol::as_table_t<std::vector<std::vector<float>>> matrix, const std::string& constness) {
        this->initMat(state, transition, matrix.value(), constness);
    });
}

std::vector<float> BcifsBuilder::parseColor(sol::object obj) {
    if (!obj.is<sol::table>()) {
        throw sol::error(std::string("The color must be an array of 3 floats"));
    }

    sol::table t = obj;
    std::vector<float> color;

    for (const auto& [key, value] : t) {
        if (!value.is<float>()) {
            throw sol::error(std::string("The color must be an array of 3 floats"));
        }
        color.push_back(value.as<float>());
    }

    return color;
}

StateID BcifsBuilder::getStateID(const std::string& name) {
    auto it = m_mapStates.find(name);
    if (it == m_mapStates.end()) {
        throw sol::error("State " + name + " does not exist");
    }
    return it->second;
}

TransitionID BcifsBuilder::getTransitionID(StateID stateId, const std::string& name) {
    auto itTransition = m_mapTransitions[stateId].find(name);
    if (itTransition == m_mapTransitions[stateId].end()) {
        throw sol::error("Transition " + name + " does not exist for state " + m_bcifs.automaton().findStateByID(stateId).name());
    }
    return itTransition->second;
}

void BcifsBuilder::assertStateDoesntExist(const std::string& name) {
    if (m_mapStates.find(name) != m_mapStates.end()) {
        throw sol::error("State " + name + " already exists");
    }
}

void BcifsBuilder::assertTransitionDoesntExist(StateID stateId, const std::string& name) {
    if (m_mapTransitions[stateId].find(name) != m_mapTransitions[stateId].end()) {
        throw sol::error("Transition " + name + " already exists");
    }
}

} // BCIFS
