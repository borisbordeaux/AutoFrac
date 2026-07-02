#include "app/exporter/jsonexporter.h"

#include <fstream>
#include <vector>
#include "nlohmann_json/json.hpp"

#include "app/bcifs/bcifs.h"
#include "app/bcifs/bcifsvertex.h" 
#include "app/bcifs/automaton.h"
#include "app/bcifs/transition.h"

void JSONExporter::exportJson(const std::string& filename, BCIFS::Bcifs& bcifs) {
    bcifs.faces(0);

    nlohmann::json root;
    const auto& automaton = bcifs.automaton();

    root["states"] = nlohmann::json::array();
    for (const auto& state : automaton.states()) {
        nlohmann::json s;
        auto state_id = state.id();
        s["id"] = state_id;
        s["name"] = state.name();

        if (bcifs.m_mapPrimitivesMat.count(state_id)) {
            const std::vector<arma::mat>& primitive_matrices = bcifs.m_mapPrimitivesMat.at(state_id);
            s["has_primitive"] = true;
            s["primitive_faces"] = nlohmann::json::array();

            for (const arma::mat& face_matrix : primitive_matrices) {
                nlohmann::json face_json;
                face_json["has_primitive"] = true;
                face_json["barycentric_dim"] = face_matrix.n_rows;

                auto vertices_array = nlohmann::json::array();
                for (std::size_t col = 0; col < face_matrix.n_cols; col++) {
                    auto vertex_coords = nlohmann::json::array();
                    for (std::size_t row = 0; row < face_matrix.n_rows; row++) {
                        vertex_coords.push_back(face_matrix(row, col));
                    }
                    vertices_array.push_back(vertex_coords);
                }
                face_json["vertices"] = vertices_array;
                s["primitive_faces"].push_back(face_json);
            }
        } else {
            s["has_primitive"] = false;
        }
        
        root["states"].push_back(s);
    }

    root["transitions"] = nlohmann::json::array();
    for (const auto& transition : automaton.transitions()) {
        nlohmann::json t;
        t["id"] = transition.id();
        t["name"] = transition.name();
        t["from_state_id"] = transition.from();
        t["to_state_id"] = transition.to();

        std::string typeStr = "UNKNOWN";
        switch (transition.type()) {
            case BCIFS::TransitionType::BOUNDARY:    typeStr = "BOUNDARY"; break;
            case BCIFS::TransitionType::SUBDIVISION: typeStr = "SUBDIVISION"; break;
            case BCIFS::TransitionType::INTERNAL:    typeStr = "INTERNAL"; break;
            case BCIFS::TransitionType::PERMUTATION: typeStr = "PERMUTATION"; break;
        }
        t["type"] = typeStr;

        try {
            const arma::mat& matrix = bcifs.getOperatorMat(transition.id());

            t["matrix_rows"] = matrix.n_rows;
            t["matrix_cols"] = matrix.n_cols;

            auto jsonMatrix = nlohmann::json::array();
            for (std::size_t r = 0; r < matrix.n_rows; r++) {
                auto jsonRow = nlohmann::json::array();
                for (std::size_t c = 0; c < matrix.n_cols; c++) {
                    jsonRow.push_back(matrix(r, c));
                }
                jsonMatrix.push_back(jsonRow);
            }
            t["matrix"] = jsonMatrix;
        } catch (...) {
            t["matrix"] = nullptr;
        }

        root["transitions"].push_back(t);
    }

    std::ofstream outFile(filename);
    if (!outFile.is_open()) throw std::runtime_error("Json export: Can not open output destination file" + filename);
    outFile << root.dump(4);
}