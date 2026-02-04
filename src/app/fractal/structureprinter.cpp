#include "app/fractal/structureprinter.h"

#include "app/fractal/face.h"
#include "app/fractal/structure.h"
#include "app/utils/utils.h"

namespace frac {

StructurePrinter::StructurePrinter(Structure const& structure, bool planarControlPoints, std::string filename) :
    m_structure(structure), m_planarControlPoints(planarControlPoints), m_filename(std::move(filename)) {}

std::string StructurePrinter::exportStruct() {
    m_filePrinter.reset();
    this->print_vertex_state();
    m_filePrinter.append_nl("------------------------------");
    m_filePrinter.append_nl("-- all edges states");
    auto edges = m_structure.allEdges();
    for (auto const& edge : edges.data()) {
        this->print_decl_of_edge(edge);
    }

    m_filePrinter.append_nl("------------------------------");
    m_filePrinter.append_nl("-- all edges impl");
    for (auto const& edge : edges.data()) {
        this->print_impl_of_edge(edge);
    }

    m_filePrinter.append_nl("------------------------------");
    m_filePrinter.append_nl("-- all cells states");
    auto cells = m_structure.allFaces();
    for (auto const& c : cells.data()) {
        m_filePrinter.append_nl("-- " + c.toString());
        m_filePrinter.append_nl("state('" + c.name() + "', 0)");
    }

    m_filePrinter.append_nl("------------------------------");
    m_filePrinter.append_nl("-- subd of init");
    this->print_init_subds();

    m_filePrinter.append_nl("------------------------------");
    m_filePrinter.append_nl("-- edges of all states");
    for (auto const& c : cells.data()) {
        this->print_edges_of_cell(c);
    }

    m_filePrinter.append_nl("------------------------------");
    m_filePrinter.append_nl("-- subdivisions of all states");
    for (auto const& c : cells.data()) {
        this->print_subd_of_cell(c);
    }

    m_filePrinter.append_nl("------------------------------");
    m_filePrinter.append_nl("-- spaces of all states");
    for (auto const& c : cells.data()) {
        this->print_space_of_cell(c);
    }

    m_filePrinter.append_nl("------------------------------");
    m_filePrinter.append_nl("-- grid of all states");
    for (auto const& c : cells.data()) {
        m_filePrinter.append_nl("gridFromBoundary('" + c.name() + "')");
    }

    m_filePrinter.append_nl("------------------------------");
    m_filePrinter.append_nl("-- prim of all states");
    for (auto const& c : cells.data()) {
        this->print_prim_of_cell(c);
    }

    m_filePrinter.append_nl("------------------------------");
    m_filePrinter.append_nl("-- constraints of all states");
    for (auto const& c : cells.data()) {
        m_filePrinter.append_nl("-- incidence constraints");
        m_filePrinter.append(Face::s_incidenceConstraints[c.name()]);
        m_filePrinter.append_nl("-- adjacency constraints");
        if (Face::s_adjacencyConstraints.find(c.name()) != Face::s_adjacencyConstraints.end()) {
            m_filePrinter.append(Face::s_adjacencyConstraints[c.name()]);
        }
        m_filePrinter.append_nl("-- edges adjacency constraints");
        this->print_edge_adjacencies_of_cell(c);
    }

    m_filePrinter.append_nl("------------------------------");
    m_filePrinter.append_nl("-- constraints on init cells");
    m_filePrinter.append(m_structure.strAdjacencies());

    if (m_filename != "") {
        m_filePrinter.printToFile(m_filename);
    }

    return m_filePrinter.text();
}

void StructurePrinter::print_vertex_state() {
    m_filePrinter.append_nl("state('s', 1)");
    m_filePrinter.append_nl("subdivision('s0', 's', 's')");
}

void StructurePrinter::print_decl_of_edge(const Edge& edge) {
    if (edge.edgeType() == EdgeType::CANTOR) {
        if (edge.isDelay()) {
            this->print_delay_cantor_decl(edge.nbSubdivisions(), edge.delay());
        } else {
            this->print_cantor_n_state_decl(edge.nbSubdivisions());
        }
    } else {
        if (edge.isDelay()) {
            this->print_delay_bezier_decl(edge.nbSubdivisions(), edge.delay());
        } else {
            this->print_bezier_state_decl(edge.nbSubdivisions());
        }
    }
}

void StructurePrinter::print_delay_cantor_decl(unsigned int n, unsigned int delay_count) {
    m_filePrinter.append_nl("state('C" + std::to_string(n) + "_" + std::to_string(delay_count) + "', " + (m_structure.cantorType() == CantorType::Cubic_Cantor ? "2" : (m_structure.cantorType() == CantorType::Quadratic_Cantor ? "1" : "0")) + ")");
    m_filePrinter.append_nl("boundary('b0', 'C" + std::to_string(n) + "_" + std::to_string(delay_count) + "', 's')");
    m_filePrinter.append_nl("boundary('b1', 'C" + std::to_string(n) + "_" + std::to_string(delay_count) + "', 's')");
    m_filePrinter.append_nl("permutation('permut', 'C" + std::to_string(n) + "_" + std::to_string(delay_count) + "', 'C" + std::to_string(n) + "_" + std::to_string(delay_count) + "')");
}

void StructurePrinter::print_cantor_n_state_decl(unsigned int n) {
    m_filePrinter.append_nl("state('C" + std::to_string(n) + "', " + (m_structure.cantorType() == CantorType::Cubic_Cantor ? "2" : (m_structure.cantorType() == CantorType::Quadratic_Cantor ? "1" : "0")) + ")");
    m_filePrinter.append_nl("boundary('b0', 'C" + std::to_string(n) + "', 's')");
    m_filePrinter.append_nl("boundary('b1', 'C" + std::to_string(n) + "', 's')");
    m_filePrinter.append_nl("permutation('permut', 'C" + std::to_string(n) + "', 'C" + std::to_string(n) + "')");
}

void StructurePrinter::print_delay_bezier_decl(unsigned int n, unsigned int delay_count) {
    m_filePrinter.append_nl("state('B" + std::to_string(n) + "_" + std::to_string(delay_count) + "', " + (m_structure.bezierType() == BezierType::Cubic_Bezier ? "2" : (m_structure.bezierType() == BezierType::Quadratic_Bezier ? "1" : "0")) + ")");
    m_filePrinter.append_nl("boundary('b0', 'B" + std::to_string(n) + "_" + std::to_string(delay_count) + "', 's')");
    m_filePrinter.append_nl("boundary('b1', 'B" + std::to_string(n) + "_" + std::to_string(delay_count) + "', 's')");
    m_filePrinter.append_nl("permutation('permut', 'B" + std::to_string(n) + "_" + std::to_string(delay_count) + "', 'B" + std::to_string(n) + "_" + std::to_string(delay_count) + "')");
}

void StructurePrinter::print_bezier_state_decl(unsigned int n) {
    m_filePrinter.append_nl("state('B" + std::to_string(n) + "', " + (m_structure.bezierType() == BezierType::Cubic_Bezier ? "2" : (m_structure.bezierType() == BezierType::Quadratic_Bezier ? "1" : "0")) + ")");
    m_filePrinter.append_nl("boundary('b0', 'B" + std::to_string(n) + "', 's')");
    m_filePrinter.append_nl("boundary('b1', 'B" + std::to_string(n) + "', 's')");
    m_filePrinter.append_nl("permutation('permut', 'B" + std::to_string(n) + "', 'B" + std::to_string(n) + "')");
}

void StructurePrinter::print_impl_of_edge(const Edge& edge) {
    if (edge.edgeType() == EdgeType::CANTOR) {
        if (edge.isDelay()) {
            this->print_delay_cantor_impl(edge.nbSubdivisions(), edge.delay());
        } else {
            this->print_cantor_n_state_impl(edge.nbSubdivisions());
        }
    } else {
        if (edge.isDelay()) {
            this->print_delay_bezier_impl(edge.nbSubdivisions(), edge.delay());
        } else {
            this->print_bezier_state_impl(edge.nbSubdivisions());
        }
    }
}

void StructurePrinter::print_delay_cantor_impl(unsigned int n, unsigned int delay_count) {
    m_filePrinter.append_nl("subdivision('s0', 'C" + std::to_string(n) + "_" + std::to_string(delay_count) + "', '" + (delay_count > 1 ? "C" + std::to_string(n) + "_" + std::to_string(delay_count - 1) : "C" + std::to_string(n)) + "')");
    if (m_structure.cantorType() == CantorType::Linear_Cantor) {
        m_filePrinter.append_nl("space('C" + std::to_string(n) + "_" + std::to_string(delay_count) + "', { 'b0', 'b1' })");
    } else if (m_structure.cantorType() == CantorType::Quadratic_Cantor) {
        m_filePrinter.append_nl("space('C" + std::to_string(n) + "_" + std::to_string(delay_count) + "', { 'b0', 'intern_0', 'b1' })");
    } else {
        //cubic
        m_filePrinter.append_nl("space('C" + std::to_string(n) + "_" + std::to_string(delay_count) + "', { 'b0', 'intern_0', 'intern_1', 'b1' })");
    }
    m_filePrinter.append_nl("constraint('C" + std::to_string(n) + "_" + std::to_string(delay_count) + "', { 'permut', 'b0' }, { 'b1' })");
    m_filePrinter.append_nl("constraint('C" + std::to_string(n) + "_" + std::to_string(delay_count) + "', { 'permut', 'b1' }, { 'b0' })");

    //permut intern
    if (m_structure.cantorType() == CantorType::Quadratic_Cantor) {
        m_filePrinter.append_nl("constraint('C" + std::to_string(n) + "_" + std::to_string(delay_count) + "', { 'permut', 'intern_0' }, { 'intern_0' })");
    } else if (m_structure.cantorType() == CantorType::Cubic_Cantor) {
        m_filePrinter.append_nl("constraint('C" + std::to_string(n) + "_" + std::to_string(delay_count) + "', { 'permut', 'intern_0' }, { 'intern_1' })");
        m_filePrinter.append_nl("constraint('C" + std::to_string(n) + "_" + std::to_string(delay_count) + "', { 'permut', 'intern_1' }, { 'intern_0' })");
    }

    m_filePrinter.append_nl("constraint('C" + std::to_string(n) + "_" + std::to_string(delay_count) + "', { 'permut', 's0' }, { 's0', 'permut' })");
    m_filePrinter.append_nl("constraint('C" + std::to_string(n) + "_" + std::to_string(delay_count) + "', { 'b0', 's0' }, { 's0', 'b0' })");
    m_filePrinter.append_nl("constraint('C" + std::to_string(n) + "_" + std::to_string(delay_count) + "', { 'b1', 's0' }, { 's0', 'b1' })");

    if (m_structure.cantorType() == CantorType::Linear_Cantor) {
        m_filePrinter.append_nl("grid('C" + std::to_string(n) + "_" + std::to_string(delay_count) + "', { { { 'b0' }, { 'b1' } } })");
    } else if (m_structure.cantorType() == CantorType::Quadratic_Cantor) {
        m_filePrinter.append_nl("grid('C" + std::to_string(n) + "_" + std::to_string(delay_count) + "', { { { 'b0' }, { 'intern_0' }, { 'b1' } } })");
    } else {
        //cubic
        m_filePrinter.append_nl("grid('C" + std::to_string(n) + "_" + std::to_string(delay_count) + "', { { { 'b0' }, { 'intern_0' }, { 'intern_1' }, { 'b1' } } })");
    }

    //matrices for intern points
    if (m_structure.cantorType() == CantorType::Cubic_Cantor) {
        m_filePrinter.append_nl("initMat('C" + std::to_string(n) + "_" + std::to_string(delay_count) + "', 's0', {");
        m_filePrinter.append_nl("    { 1, 0, 0, 0 },");
        m_filePrinter.append_nl("    { 0, 1, 0, 0 },");
        m_filePrinter.append_nl("    { 0, 0, 1, 0 },");
        m_filePrinter.append_nl("    { 0, 0, 0, 1 }");
        m_filePrinter.append_nl("}, 'CONST')");
    } else if (m_structure.cantorType() == CantorType::Quadratic_Cantor) {
        m_filePrinter.append_nl("initMat('C" + std::to_string(n) + "_" + std::to_string(delay_count) + "', 's0', {");
        m_filePrinter.append_nl("    { 1, 0, 0 },");
        m_filePrinter.append_nl("    { 0, 1, 0 },");
        m_filePrinter.append_nl("    { 0, 0, 1 },");
        m_filePrinter.append_nl("}, 'CONST')");
    }
}

void StructurePrinter::print_cantor_n_state_impl(unsigned int n) {
    for (unsigned int i = 0; i < n; ++i) {
        m_filePrinter.append_nl("subdivision('s" + std::to_string(i) + "', 'C" + std::to_string(n) + "', 'C" + std::to_string(n) + "')");
    }

    if (m_structure.cantorType() == CantorType::Linear_Cantor) {
        m_filePrinter.append_nl("space('C" + std::to_string(n) + "', { 'b0', 'b1' })");
    } else if (m_structure.cantorType() == CantorType::Quadratic_Cantor) {
        m_filePrinter.append_nl("space('C" + std::to_string(n) + "', { 'b0', 'intern_0', 'b1' })");
    } else {
        //cubic
        m_filePrinter.append_nl("space('C" + std::to_string(n) + "', { 'b0', 'intern_0', 'intern_1', 'b1' })");
    }

    m_filePrinter.append_nl("constraint('C" + std::to_string(n) + "', { 'permut', 'b0' }, { 'b1' })");
    m_filePrinter.append_nl("constraint('C" + std::to_string(n) + "', { 'permut', 'b1' }, { 'b0' })");
    for (unsigned int i = 0; i < n; ++i) {
        m_filePrinter.append_nl("constraint('C" + std::to_string(n) + "', { 'permut', 's" + std::to_string(i) + "' }, { 's" + std::to_string(n - i - 1) + "', 'permut' })");
    }

    //permut intern
    if (m_structure.cantorType() == CantorType::Quadratic_Cantor) {
        m_filePrinter.append_nl("constraint('C" + std::to_string(n) + "', { 'permut', 'intern_0' }, { 'intern_0' })");
    } else if (m_structure.cantorType() == CantorType::Cubic_Cantor) {
        m_filePrinter.append_nl("constraint('C" + std::to_string(n) + "', { 'permut', 'intern_0' }, { 'intern_1' })");
        m_filePrinter.append_nl("constraint('C" + std::to_string(n) + "', { 'permut', 'intern_1' }, { 'intern_0' })");
    }

    m_filePrinter.append_nl("constraint('C" + std::to_string(n) + "', { 'b0', 's0' }, { 's0', 'b0' })");
    m_filePrinter.append_nl("constraint('C" + std::to_string(n) + "', { 'b1', 's0' }, { 's" + std::to_string(n - 1) + "', 'b1' })");

    if (m_structure.cantorType() == CantorType::Linear_Cantor) {
        m_filePrinter.append_nl("grid('C" + std::to_string(n) + "', { { { 'b0' }, { 'b1' } } })");
    } else if (m_structure.cantorType() == CantorType::Quadratic_Cantor) {
        m_filePrinter.append_nl("grid('C" + std::to_string(n) + "', { { { 'b0' }, { 'intern_0' }, { 'b1' } } })");
    } else {
        //cubic
        m_filePrinter.append_nl("grid('C" + std::to_string(n) + "', { { { 'b0' }, { 'intern_0' }, { 'intern_1' }, { 'b1' } } })");
    }

    //matrices
    if (m_structure.cantorType() == CantorType::Linear_Cantor) {
        for (unsigned int i = 0; i < n; ++i) {
            // for each subdivision T0, T1, ... Tn-1
            std::vector<float> t = frac::utils::get_cantor_linear_transformation(i, n);
            m_filePrinter.append_nl("initMat('C" + std::to_string(n) + "', 's" + std::to_string(i) + "', {");
            m_filePrinter.append_nl("    { " + frac::utils::to_string(t[0]) + ", " + frac::utils::to_string(t[1]) + " },");
            m_filePrinter.append_nl("    { " + frac::utils::to_string(t[2]) + ", " + frac::utils::to_string(t[3]) + " },");
            m_filePrinter.append_nl("}, 'CONST')");
        }
    } else if (m_structure.cantorType() == CantorType::Quadratic_Cantor) {
        for (unsigned int i = 0; i < n; ++i) {
            // for each subdivision T0, T1, ... Tn-1
            std::vector<float> t = frac::utils::get_cantor_quadratic_transformation(i, n);
            m_filePrinter.append_nl("initMat('C" + std::to_string(n) + "', 's" + std::to_string(i) + "', {");
            m_filePrinter.append_nl("    { " + frac::utils::to_string(t[0]) + ", " + frac::utils::to_string(t[1]) + ", " + frac::utils::to_string(t[2]) + " },");
            m_filePrinter.append_nl("    { " + frac::utils::to_string(t[3]) + ", " + frac::utils::to_string(t[4]) + ", " + frac::utils::to_string(t[5]) + " },");
            m_filePrinter.append_nl("    { " + frac::utils::to_string(t[6]) + ", " + frac::utils::to_string(t[7]) + ", " + frac::utils::to_string(t[8]) + " },");
            m_filePrinter.append_nl("}, 'CONST')");
        }
    } else {
        //cubic
        for (unsigned int i = 0; i < n; ++i) {
            // for each subdivision T0, T1, ... Tn-1
            std::vector<float> t = frac::utils::get_cantor_cubic_transformation(i, n);
            m_filePrinter.append_nl("initMat('C" + std::to_string(n) + "', 's" + std::to_string(i) + "', {");
            m_filePrinter.append_nl("    { " + frac::utils::to_string(t[0]) + ", " + frac::utils::to_string(t[1]) + ", " + frac::utils::to_string(t[2]) + ", " + frac::utils::to_string(t[3]) + " },");
            m_filePrinter.append_nl("    { " + frac::utils::to_string(t[4]) + ", " + frac::utils::to_string(t[5]) + ", " + frac::utils::to_string(t[6]) + ", " + frac::utils::to_string(t[7]) + " },");
            m_filePrinter.append_nl("    { " + frac::utils::to_string(t[8]) + ", " + frac::utils::to_string(t[9]) + ", " + frac::utils::to_string(t[10]) + ", " + frac::utils::to_string(t[11]) + " },");
            m_filePrinter.append_nl("    { " + frac::utils::to_string(t[12]) + ", " + frac::utils::to_string(t[13]) + ", " + frac::utils::to_string(t[14]) + ", " + frac::utils::to_string(t[15]) + " },");
            m_filePrinter.append_nl("}, 'CONST')");
        }
    }
}

void StructurePrinter::print_delay_bezier_impl(unsigned int n, unsigned int delay_count) {
    m_filePrinter.append_nl("subdivision('s0', 'B" + std::to_string(n) + "_" + std::to_string(delay_count) + "', '" + (delay_count > 1 ? "B" + std::to_string(n) + "_" + std::to_string(delay_count - 1) : "B" + std::to_string(n)) + "')");
    if (m_structure.bezierType() == BezierType::Linear_Bezier) {
        m_filePrinter.append_nl("space('B" + std::to_string(n) + "_" + std::to_string(delay_count) + "', { 'b0', 'b1' })");
    } else if (m_structure.bezierType() == BezierType::Quadratic_Bezier) {
        m_filePrinter.append_nl("space('B" + std::to_string(n) + "_" + std::to_string(delay_count) + "', { 'b0', 'intern_0', 'b1' })");
    } else {
        //cubic
        m_filePrinter.append_nl("space('B" + std::to_string(n) + "_" + std::to_string(delay_count) + "', { 'b0', 'intern_0', 'intern_1', 'b1' })");
    }
    m_filePrinter.append_nl("constraint('B" + std::to_string(n) + "_" + std::to_string(delay_count) + "', { 'permut', 'b0' }, { 'b1' })");
    m_filePrinter.append_nl("constraint('B" + std::to_string(n) + "_" + std::to_string(delay_count) + "', { 'permut', 'b1' }, { 'b0' })");

    //permut intern
    if (m_structure.bezierType() == BezierType::Quadratic_Bezier) {
        m_filePrinter.append_nl("constraint('B" + std::to_string(n) + "_" + std::to_string(delay_count) + "', { 'permut', 'intern_0' }, { 'intern_0' })");
    } else if (m_structure.bezierType() == BezierType::Cubic_Bezier) {
        m_filePrinter.append_nl("constraint('B" + std::to_string(n) + "_" + std::to_string(delay_count) + "', { 'permut', 'intern_0' }, { 'intern_1' })");
        m_filePrinter.append_nl("constraint('B" + std::to_string(n) + "_" + std::to_string(delay_count) + "', { 'permut', 'intern_1' }, { 'intern_0' })");
    }

    m_filePrinter.append_nl("constraint('B" + std::to_string(n) + "_" + std::to_string(delay_count) + "', { 'permut', 's0' }, { 's0', 'permut' })");
    m_filePrinter.append_nl("constraint('B" + std::to_string(n) + "_" + std::to_string(delay_count) + "', { 'b0', 's0' }, { 's0', 'b0' })");
    m_filePrinter.append_nl("constraint('B" + std::to_string(n) + "_" + std::to_string(delay_count) + "', { 'b1', 's0' }, { 's0', 'b1' })");

    if (m_structure.bezierType() == BezierType::Linear_Bezier) {
        m_filePrinter.append_nl("grid('B" + std::to_string(n) + "_" + std::to_string(delay_count) + "', { { { 'b0' }, { 'b1' } } })");
    } else if (m_structure.bezierType() == BezierType::Quadratic_Bezier) {
        m_filePrinter.append_nl("grid('B" + std::to_string(n) + "_" + std::to_string(delay_count) + "', { { { 'b0' }, { 'intern_0' }, { 'b1' } } })");
    } else {
        //cubic
        m_filePrinter.append_nl("grid('B" + std::to_string(n) + "_" + std::to_string(delay_count) + "', { { { 'b0' }, { 'intern_0' }, { 'intern_1' }, { 'b1' } } })");
    }

    //matrices for intern points
    if (m_structure.bezierType() == BezierType::Cubic_Bezier) {
        m_filePrinter.append_nl("initMat('B" + std::to_string(n) + "_" + std::to_string(delay_count) + "', 's0', {");
        m_filePrinter.append_nl("    { 1, 0, 0, 0 },");
        m_filePrinter.append_nl("    { 0, 1, 0, 0 },");
        m_filePrinter.append_nl("    { 0, 0, 1, 0 },");
        m_filePrinter.append_nl("    { 0, 0, 0, 1 }");
        m_filePrinter.append_nl("}, 'CONST')");
    } else if (m_structure.bezierType() == BezierType::Quadratic_Bezier) {
        m_filePrinter.append_nl("initMat('B" + std::to_string(n) + "_" + std::to_string(delay_count) + "', 's0', {");
        m_filePrinter.append_nl("    { 1, 0, 0 },");
        m_filePrinter.append_nl("    { 0, 1, 0 },");
        m_filePrinter.append_nl("    { 0, 0, 1 },");
        m_filePrinter.append_nl("}, 'CONST')");
    }
}

void StructurePrinter::print_bezier_state_impl(unsigned int n) {
    for (unsigned int i = 0; i < n; ++i) {
        m_filePrinter.append_nl("subdivision('s" + std::to_string(i) + "', 'B" + std::to_string(n) + "', 'B" + std::to_string(n) + "')");
    }

    if (m_structure.bezierType() == BezierType::Linear_Bezier) {
        m_filePrinter.append_nl("space('B" + std::to_string(n) + "', { 'b0', 'b1' })");
    } else if (m_structure.bezierType() == BezierType::Quadratic_Bezier) {
        m_filePrinter.append_nl("space('B" + std::to_string(n) + "', { 'b0', 'intern_0', 'b1' })");
    } else {
        //cubic
        m_filePrinter.append_nl("space('B" + std::to_string(n) + "', { 'b0', 'intern_0', 'intern_1', 'b1' })");
    }

    m_filePrinter.append_nl("constraint('B" + std::to_string(n) + "', { 'permut', 'b0' }, { 'b1' })");
    m_filePrinter.append_nl("constraint('B" + std::to_string(n) + "', { 'permut', 'b1' }, { 'b0' })");
    for (unsigned int i = 0; i < n; ++i) {
        m_filePrinter.append_nl("constraint('B" + std::to_string(n) + "', { 'permut', 's" + std::to_string(i) + "' }, { 's" + std::to_string(n - i - 1) + "', 'permut' })");
    }

    //permut intern
    if (m_structure.bezierType() == BezierType::Quadratic_Bezier) {
        m_filePrinter.append_nl("constraint('B" + std::to_string(n) + "', { 'permut', 'intern_0' }, { 'intern_0' })");
    } else if (m_structure.bezierType() == BezierType::Cubic_Bezier) {
        m_filePrinter.append_nl("constraint('B" + std::to_string(n) + "', { 'permut', 'intern_0' }, { 'intern_1' })");
        m_filePrinter.append_nl("constraint('B" + std::to_string(n) + "', { 'permut', 'intern_1' }, { 'intern_0' })");
    }

    m_filePrinter.append_nl("constraint('B" + std::to_string(n) + "', { 'b0', 's0' }, { 's0', 'b0' })");
    m_filePrinter.append_nl("constraint('B" + std::to_string(n) + "', { 'b1', 's0' }, { 's" + std::to_string(n - 1) + "', 'b1' })");

    if (m_structure.bezierType() == BezierType::Linear_Bezier) {
        m_filePrinter.append_nl("grid('B" + std::to_string(n) + "', { { { 'b0' }, { 'b1' } } })");
    } else if (m_structure.bezierType() == BezierType::Quadratic_Bezier) {
        m_filePrinter.append_nl("grid('B" + std::to_string(n) + "', { { { 'b0' }, { 'intern_0' }, { 'b1' } } })");
    } else {
        //cubic
        m_filePrinter.append_nl("grid('B" + std::to_string(n) + "', { { { 'b0' }, { 'intern_0' }, { 'intern_1' }, { 'b1' } } })");
    }

    //matrices
    if (m_structure.bezierType() == BezierType::Linear_Bezier) {
        for (unsigned int i = 0; i < n; ++i) {
            // for each subdivision T0, T1, ... Tn-1
            std::vector<float> t = frac::utils::get_bezier_linear_transformation(i, n);
            m_filePrinter.append_nl("initMat('B" + std::to_string(n) + "', 's" + std::to_string(i) + "', {");
            m_filePrinter.append_nl("    { " + frac::utils::to_string(t[0]) + ", " + frac::utils::to_string(t[1]) + " },");
            m_filePrinter.append_nl("    { " + frac::utils::to_string(t[2]) + ", " + frac::utils::to_string(t[3]) + " },");
            m_filePrinter.append_nl("}, 'CONST')");
        }
    } else if (m_structure.bezierType() == BezierType::Quadratic_Bezier) {
        for (unsigned int i = 0; i < n; ++i) {
            // for each subdivision T0, T1, ... Tn-1
            std::vector<float> t = frac::utils::get_bezier_quadratic_transformation(i, n);
            m_filePrinter.append_nl("initMat('B" + std::to_string(n) + "', 's" + std::to_string(i) + "', {");
            m_filePrinter.append_nl("    { " + frac::utils::to_string(t[0]) + ", " + frac::utils::to_string(t[1]) + ", " + frac::utils::to_string(t[2]) + " },");
            m_filePrinter.append_nl("    { " + frac::utils::to_string(t[3]) + ", " + frac::utils::to_string(t[4]) + ", " + frac::utils::to_string(t[5]) + " },");
            m_filePrinter.append_nl("    { " + frac::utils::to_string(t[6]) + ", " + frac::utils::to_string(t[7]) + ", " + frac::utils::to_string(t[8]) + " },");
            m_filePrinter.append_nl("}, 'CONST')");
        }
    } else {
        //cubic
        for (unsigned int i = 0; i < n; ++i) {
            // for each subdivision T0, T1, ... Tn-1
            std::vector<float> t = frac::utils::get_bezier_cubic_transformation(i, n);
            m_filePrinter.append_nl("initMat('B" + std::to_string(n) + "', 's" + std::to_string(i) + "', {");
            m_filePrinter.append_nl("    { " + frac::utils::to_string(t[0]) + ", " + frac::utils::to_string(t[1]) + ", " + frac::utils::to_string(t[2]) + ", " + frac::utils::to_string(t[3]) + " },");
            m_filePrinter.append_nl("    { " + frac::utils::to_string(t[4]) + ", " + frac::utils::to_string(t[5]) + ", " + frac::utils::to_string(t[6]) + ", " + frac::utils::to_string(t[7]) + " },");
            m_filePrinter.append_nl("    { " + frac::utils::to_string(t[8]) + ", " + frac::utils::to_string(t[9]) + ", " + frac::utils::to_string(t[10]) + ", " + frac::utils::to_string(t[11]) + " },");
            m_filePrinter.append_nl("    { " + frac::utils::to_string(t[12]) + ", " + frac::utils::to_string(t[13]) + ", " + frac::utils::to_string(t[14]) + ", " + frac::utils::to_string(t[15]) + " },");
            m_filePrinter.append_nl("}, 'CONST')");
        }
    }
}

void StructurePrinter::print_init_subds() {
    auto const& subds = m_structure.faces();
    int i = 0;
    for (auto const& s : subds) {
        m_filePrinter.append_nl("subdivision('s" + std::to_string(i) + "', 'init', '" + s.name() + "')");
        i += 1;
    }
}

void StructurePrinter::print_edges_of_cell(Face const& cell) {
    int i = 0;
    for (auto const& edge : cell.constData()) {
        m_filePrinter.append_nl("boundary('b" + std::to_string(i) + "', '" + cell.name() + "', '" + edge.name() + "')");
        i += 1;
    }
}

void StructurePrinter::print_subd_of_cell(Face const& cell) {
    std::vector<Face> subds = cell.subdivisions();
    int i = 0;
    for (Face const& f : subds) {
        m_filePrinter.append_nl("subdivision('s" + std::to_string(i) + "', '" + cell.name() + "', '" + f.name() + "')");
        i += 1;
    }
}

void StructurePrinter::print_space_of_cell(Face const& cell) {
    m_filePrinter.append("space('" + cell.name() + "', { ");
    for (std::size_t i = 0; i < cell.len(); ++i) {
        if (i == 0) {
            m_filePrinter.append("'b" + std::to_string(i) + "'");
        } else {
            m_filePrinter.append(", 'b" + std::to_string(i) + "'");
        }
    }
    m_filePrinter.append_nl("})");
}

void StructurePrinter::print_prim_of_cell(Face const& cell) {
    m_filePrinter.append_nl("primitive('" + cell.name() + "', { {");
    for (std::size_t i = 0; i < cell.len(); ++i) {
        if (cell[i].edgeType() == EdgeType::BEZIER && cell[i].delay() == 0) {
            for (std::size_t j = 0; j < cell[i].nbSubdivisions(); ++j) {
                if (cell[i].nbSubdivisions() > 2) {
                    m_filePrinter.append_nl("    { 'b" + std::to_string(i) + "', 's" + std::to_string(j) + "', 'b0' },");
                } else {
                    for (std::size_t k = 0; k < cell[i].nbSubdivisions(); ++k) {
                        m_filePrinter.append_nl("    { 'b" + std::to_string(i) + "', 's" + std::to_string(j) + "', 's" + std::to_string(k) + "', 'b0' },");
                    }
                }
            }
        } else {
            m_filePrinter.append_nl("    { 'b" + std::to_string(i) + "', 'b0' },");
        }
    }
    m_filePrinter.append_nl("} })");
}

void StructurePrinter::print_edge_adjacencies_of_cell(Face const& cell) {
    for (std::size_t i = 0; i < cell.len(); ++i) {
        m_filePrinter.append_nl("constraint('" + cell.name() + "', { 'b" + std::to_string(i) + "', 'b1' }, { 'b" + std::to_string(utils::mod(i + 1, cell.len())) + "', 'b0' })");
    }
}

} // frac
