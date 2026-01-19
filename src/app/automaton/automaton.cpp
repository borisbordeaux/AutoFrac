#include "app/automaton/automaton.h"
#include "app/automaton/transition.h"
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <queue>
#include <unordered_set>

namespace BCIFS {

void Automaton::addState(const State& state) {
    m_states.push_back(state);
}

void Automaton::addTransition(const Transition& transition) {
    m_transitions.push_back(transition);
}

bool Automaton::containsSubdivision(const Path& path) const {
    return std::any_of(path.begin(), path.end(), [this](TransitionID id) {
        const Transition& transition = this->findTransitionByID(id);
        return transition.type() == TransitionType::SUBDIVISION;
    });
}

const Transition& Automaton::findTransitionByID(TransitionID id) const {
    for (const Transition& transition: m_transitions) {
        if (transition.id() == id) {
            return transition;
        }
    }

    throw std::runtime_error("Transition not found");
}

const State& Automaton::findStateByID(StateID id) const {
    for (const State& state: m_states) {
        if (state.id() == id) {
            return state;
        }
    }

    throw std::runtime_error("State not found");
}

std::string Automaton::toString() const {
    std::string res = "States:\n";
    for (const State& state: m_states) {
        res += state.name() + " with " + std::to_string(this->internalDimensions(state.id())) + " internal dimensions\n";
    }
    res += "\nTransitions:\n";
    for (const Transition& transition: m_transitions) {
        res += this->findStateByID(transition.from()).name() + " ";
        res += transition.toString();
        res += " " + this->findStateByID(transition.to()).name() + "\n";
    }
    res += '\n';
    return res;
}

void Automaton::print() const {
    std::cout << "States:\n";
    for (const State& state: m_states) {
        std::cout << state.name() << " with " << this->internalDimensions(state.id()) << " internal dimensions\n";
    }
    std::cout << "\nTransitions:\n";
    for (const Transition& transition: m_transitions) {
        std::cout << this->findStateByID(transition.from()).name() << " ";
        transition.print();
        std::cout << " " << this->findStateByID(transition.to()).name() << "\n";
    }
    std::cout << std::endl;
}

std::size_t Automaton::internalDimensions(StateID id) const {
    std::size_t res = 0;
    for (const Transition& transition: m_transitions) {
        if (transition.type() == TransitionType::INTERNAL && transition.from() == id) {
            res++;
        }
    }
    return res;
}

void Automaton::check() const {
    if (m_states.size() < 2) {
        throw std::runtime_error("Not enough states.");
    }
    if (m_transitions.size() < 2) {
        throw std::runtime_error("Not enough transitions.");
    }

    for (const Transition& transition: m_transitions) {
        bool beginFound = false;
        bool endFound = false;
        for (const State& state: m_states) {
            if (state.id() == transition.from()) {
                beginFound = true;
            }
            if (state.id() == transition.to()) {
                endFound = true;
            }
        }
        if (!beginFound || !endFound) {
            throw std::runtime_error("Transition not found");
        }
    }
}

std::vector<TransitionID> Automaton::boundaryTransitionsOf(StateID id) const {
    std::vector<TransitionID> res;
    for (const Transition& transition: m_transitions) {
        if (transition.from() == id && transition.type() == TransitionType::BOUNDARY) {
            res.push_back(transition.id());
        }
    }
    return res;
}

std::vector<StateID> Automaton::boundaryStatesOf(StateID id) const {
    std::vector<StateID> res;
    for (const Transition& transition: m_transitions) {
        if (transition.from() == id && transition.type() == TransitionType::BOUNDARY) {
            res.push_back(transition.to());
        }
    }
    return res;
}

std::vector<TransitionID> Automaton::internalTransitionsOf(StateID id) const {
    std::vector<TransitionID> res;
    for (const Transition& transition: m_transitions) {
        if (transition.from() == id && transition.type() == TransitionType::INTERNAL) {
            res.push_back(transition.id());
        }
    }
    return res;
}

std::vector<TransitionID> Automaton::boundaryAndInternalTransitionsOf(StateID id) const {
    std::vector<TransitionID> res;
    for (const Transition& transition: m_transitions) {
        if (transition.from() == id && (transition.type() == TransitionType::BOUNDARY || transition.type() == TransitionType::INTERNAL)) {
            res.push_back(transition.id());
        }
    }
    return res;
}

void Automaton::reset() {
    m_states.clear();
    m_transitions.clear();
}

std::vector<TransitionID> Automaton::subdivisionTransitionsOf(StateID id) const {
    std::vector<TransitionID> res;
    for (const Transition& transition: m_transitions) {
        if (transition.from() == id && transition.type() == TransitionType::SUBDIVISION) {
            res.push_back(transition.id());
        }
    }
    return res;
}

std::vector<TransitionID> Automaton::subdivisionTransitionsTo(StateID id, StateID ignoredId) const {
    std::vector<TransitionID> res;
    for (const Transition& transition: m_transitions) {
        if (transition.to() == id && transition.type() == TransitionType::SUBDIVISION && transition.from() != ignoredId) {
            res.push_back(transition.id());
        }
    }
    return res;
}

std::vector<Path> Automaton::allSubdivisionPaths(StateID from, int depth) const {
    std::vector<Path> res;
    Path currentPath;

    this->dfs(from, 0, depth, currentPath, res);

    return res;
}

std::unordered_map<StateID, Path> Automaton::shortestPaths(StateID from) const {
    std::unordered_map<StateID, Path> result;
    std::unordered_set<StateID> visited;
    std::unordered_map<StateID, TransitionID> parent;

    std::queue<StateID> q;

    visited.insert(from);
    q.push(from);

    while (!q.empty()) {
        StateID current = q.front();
        q.pop();

        for (const TransitionID& transitionId : this->subdivisionTransitionsOf(current)) {
            Transition transition = this->findTransitionByID(transitionId);
            StateID next = transition.to();

            if (visited.count(next) == 0) {
                visited.insert(next);
                parent[next] = transitionId;
                q.push(next);
            }
        }
    }

    // Path reconstruction
    for (const auto& [node, _] : parent) {
        Path path;
        StateID cur = node;

        while (cur != from) {
            const TransitionID& transitionId = parent[cur];
            path.push_back(transitionId);
            Transition transition = this->findTransitionByID(transitionId);
            cur = transition.from();
        }

        std::reverse(path.begin(), path.end());
        result[node] = path;
    }

    return result;
}

void Automaton::dfs(StateID from, int depth, int maxDepth, Path& currentPath, std::vector<Path>& result) const {
    if (depth == maxDepth) {
        result.push_back(currentPath); // copie
        return;
    }

    for (TransitionID transitionId: this->subdivisionTransitionsOf(from)) {
        currentPath.push_back(transitionId);
        const Transition& t = findTransitionByID(transitionId);
        this->dfs(t.to(), depth + 1, maxDepth, currentPath, result);
        currentPath.pop_back();
    }
}

} // BCIFS