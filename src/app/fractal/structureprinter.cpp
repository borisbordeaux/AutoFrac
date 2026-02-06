#include "app/fractal/structureprinter.h"

#include "app/fractal/face.h"
#include "app/fractal/structure.h"
#include "app/utils/utils.h"

namespace frac {

StructurePrinter::StructurePrinter(Structure const& structure, bool planarControlPoints, std::string filename) :
    m_structure(structure), m_planarControlPoints(planarControlPoints), m_filename(std::move(filename)) {}

std::string StructurePrinter::exportStruct() {
    m_filePrinter.reset();
    this->printVertexState();
    m_filePrinter.appendNewLine("------------------------------");
    m_filePrinter.appendNewLine("-- all edges states");
    auto edges = m_structure.allEdges();
    for (auto const& edge : edges.data()) {
        this->printDeclOfEdge(edge);
    }

    m_filePrinter.appendNewLine("------------------------------");
    m_filePrinter.appendNewLine("-- all edges impl");
    for (auto const& edge : edges.data()) {
        this->printImplOfEdge(edge);
    }

    m_filePrinter.appendNewLine("------------------------------");
    m_filePrinter.appendNewLine("-- all cells states");
    auto cells = m_structure.allFaces();
    for (auto const& c : cells.data()) {
        m_filePrinter.appendNewLine("-- " + c.toString());
        m_filePrinter.appendNewLine("state('" + c.name() + "', 0)");
    }

    m_filePrinter.appendNewLine("------------------------------");
    m_filePrinter.appendNewLine("-- subd of init");
    this->printInitSubds();

    m_filePrinter.appendNewLine("------------------------------");
    m_filePrinter.appendNewLine("-- edges of all states");
    for (auto const& c : cells.data()) {
        this->printEdgesOfCell(c);
    }

    m_filePrinter.appendNewLine("------------------------------");
    m_filePrinter.appendNewLine("-- subdivisions of all states");
    for (auto const& c : cells.data()) {
        this->printSubdOfCell(c);
    }

    m_filePrinter.appendNewLine("------------------------------");
    m_filePrinter.appendNewLine("-- spaces of all states");
    for (auto const& c : cells.data()) {
        this->printSpaceOfCell(c);
    }

    m_filePrinter.appendNewLine("------------------------------");
    m_filePrinter.appendNewLine("-- grid of all states");
    for (auto const& c : cells.data()) {
        m_filePrinter.appendNewLine("gridFromBoundary('" + c.name() + "')");
    }

    m_filePrinter.appendNewLine("------------------------------");
    m_filePrinter.appendNewLine("-- prim of all states");
    for (auto const& c : cells.data()) {
        this->printPrimOfCell(c);
    }

    m_filePrinter.appendNewLine("------------------------------");
    m_filePrinter.appendNewLine("-- constraints of all states");
    for (auto const& c : cells.data()) {
        m_filePrinter.appendNewLine("-- incidence constraints");
        m_filePrinter.append(Face::s_incidenceConstraints[c.name()]);
        m_filePrinter.appendNewLine("-- adjacency constraints");
        if (Face::s_adjacencyConstraints.find(c.name()) != Face::s_adjacencyConstraints.end()) {
            m_filePrinter.append(Face::s_adjacencyConstraints[c.name()]);
        }
        m_filePrinter.appendNewLine("-- edges adjacency constraints");
        this->printEdgeAdjacenciesOfCell(c);
    }

    m_filePrinter.appendNewLine("------------------------------");
    m_filePrinter.appendNewLine("-- constraints on init cells");
    m_filePrinter.append(m_structure.strAdjacencies());

    if (m_filename != "") {
        m_filePrinter.printToFile(m_filename);
    }

    return m_filePrinter.text();
}

void StructurePrinter::printVertexState() {
    m_filePrinter.appendNewLine("state('s', 1)");
    m_filePrinter.appendNewLine("subdivision('s0', 's', 's')");
}

void StructurePrinter::printDeclOfEdge(const Edge& edge) {
    if (edge.edgeType() == EdgeType::CANTOR) {
        if (edge.isDelay()) {
            this->printDelayCantorDecl(edge.nbSubdivisions(), edge.delay());
        } else {
            this->printCantorDecl(edge.nbSubdivisions());
        }
    } else {
        if (edge.isDelay()) {
            this->printDelayBezierDecl(edge.nbSubdivisions(), edge.delay());
        } else {
            this->printBezierDecl(edge.nbSubdivisions());
        }
    }
}

void StructurePrinter::printDelayCantorDecl(unsigned int n, unsigned int delay_count) {
    m_filePrinter.appendNewLine("state('C" + std::to_string(n) + "_" + std::to_string(delay_count) + "', " + (m_structure.cantorType() == CantorType::Cubic_Cantor ? "2" : (m_structure.cantorType() == CantorType::Quadratic_Cantor ? "1" : "0")) + ")");
    m_filePrinter.appendNewLine("boundary('b0', 'C" + std::to_string(n) + "_" + std::to_string(delay_count) + "', 's')");
    m_filePrinter.appendNewLine("boundary('b1', 'C" + std::to_string(n) + "_" + std::to_string(delay_count) + "', 's')");
    m_filePrinter.appendNewLine("permutation('permut', 'C" + std::to_string(n) + "_" + std::to_string(delay_count) + "', 'C" + std::to_string(n) + "_" + std::to_string(delay_count) + "')");
}

void StructurePrinter::printCantorDecl(unsigned int n) {
    m_filePrinter.appendNewLine("state('C" + std::to_string(n) + "', " + (m_structure.cantorType() == CantorType::Cubic_Cantor ? "2" : (m_structure.cantorType() == CantorType::Quadratic_Cantor ? "1" : "0")) + ")");
    m_filePrinter.appendNewLine("boundary('b0', 'C" + std::to_string(n) + "', 's')");
    m_filePrinter.appendNewLine("boundary('b1', 'C" + std::to_string(n) + "', 's')");
    m_filePrinter.appendNewLine("permutation('permut', 'C" + std::to_string(n) + "', 'C" + std::to_string(n) + "')");
}

void StructurePrinter::printDelayBezierDecl(unsigned int n, unsigned int delay_count) {
    m_filePrinter.appendNewLine("state('B" + std::to_string(n) + "_" + std::to_string(delay_count) + "', " + (m_structure.bezierType() == BezierType::Cubic_Bezier ? "2" : (m_structure.bezierType() == BezierType::Quadratic_Bezier ? "1" : "0")) + ")");
    m_filePrinter.appendNewLine("boundary('b0', 'B" + std::to_string(n) + "_" + std::to_string(delay_count) + "', 's')");
    m_filePrinter.appendNewLine("boundary('b1', 'B" + std::to_string(n) + "_" + std::to_string(delay_count) + "', 's')");
    m_filePrinter.appendNewLine("permutation('permut', 'B" + std::to_string(n) + "_" + std::to_string(delay_count) + "', 'B" + std::to_string(n) + "_" + std::to_string(delay_count) + "')");
}

void StructurePrinter::printBezierDecl(unsigned int n) {
    m_filePrinter.appendNewLine("state('B" + std::to_string(n) + "', " + (m_structure.bezierType() == BezierType::Cubic_Bezier ? "2" : (m_structure.bezierType() == BezierType::Quadratic_Bezier ? "1" : "0")) + ")");
    m_filePrinter.appendNewLine("boundary('b0', 'B" + std::to_string(n) + "', 's')");
    m_filePrinter.appendNewLine("boundary('b1', 'B" + std::to_string(n) + "', 's')");
    m_filePrinter.appendNewLine("permutation('permut', 'B" + std::to_string(n) + "', 'B" + std::to_string(n) + "')");
}

void StructurePrinter::printImplOfEdge(const Edge& edge) {
    if (edge.edgeType() == EdgeType::CANTOR) {
        if (edge.isDelay()) {
            this->printDelayCantorImpl(edge.nbSubdivisions(), edge.delay());
        } else {
            this->printCantorImpl(edge.nbSubdivisions());
        }
    } else {
        if (edge.isDelay()) {
            this->printDelayBezierImpl(edge.nbSubdivisions(), edge.delay());
        } else {
            this->printBezierImpl(edge.nbSubdivisions());
        }
    }
}

void StructurePrinter::printDelayCantorImpl(unsigned int n, unsigned int delay_count) {
    m_filePrinter.appendNewLine("subdivision('s0', 'C" + std::to_string(n) + "_" + std::to_string(delay_count) + "', '" + (delay_count > 1 ? "C" + std::to_string(n) + "_" + std::to_string(delay_count - 1) : "C" + std::to_string(n)) + "')");
    if (m_structure.cantorType() == CantorType::Linear_Cantor) {
        m_filePrinter.appendNewLine("space('C" + std::to_string(n) + "_" + std::to_string(delay_count) + "', { 'b0', 'b1' })");
    } else if (m_structure.cantorType() == CantorType::Quadratic_Cantor) {
        m_filePrinter.appendNewLine("space('C" + std::to_string(n) + "_" + std::to_string(delay_count) + "', { 'b0', 'intern_0', 'b1' })");
    } else {
        //cubic
        m_filePrinter.appendNewLine("space('C" + std::to_string(n) + "_" + std::to_string(delay_count) + "', { 'b0', 'intern_0', 'intern_1', 'b1' })");
    }
    m_filePrinter.appendNewLine("constraint('C" + std::to_string(n) + "_" + std::to_string(delay_count) + "', { 'permut', 'b0' }, { 'b1' })");
    m_filePrinter.appendNewLine("constraint('C" + std::to_string(n) + "_" + std::to_string(delay_count) + "', { 'permut', 'b1' }, { 'b0' })");

    //permut intern
    if (m_structure.cantorType() == CantorType::Quadratic_Cantor) {
        m_filePrinter.appendNewLine("constraint('C" + std::to_string(n) + "_" + std::to_string(delay_count) + "', { 'permut', 'intern_0' }, { 'intern_0' })");
    } else if (m_structure.cantorType() == CantorType::Cubic_Cantor) {
        m_filePrinter.appendNewLine("constraint('C" + std::to_string(n) + "_" + std::to_string(delay_count) + "', { 'permut', 'intern_0' }, { 'intern_1' })");
        m_filePrinter.appendNewLine("constraint('C" + std::to_string(n) + "_" + std::to_string(delay_count) + "', { 'permut', 'intern_1' }, { 'intern_0' })");
    }

    m_filePrinter.appendNewLine("constraint('C" + std::to_string(n) + "_" + std::to_string(delay_count) + "', { 'permut', 's0' }, { 's0', 'permut' })");
    m_filePrinter.appendNewLine("constraint('C" + std::to_string(n) + "_" + std::to_string(delay_count) + "', { 'b0', 's0' }, { 's0', 'b0' })");
    m_filePrinter.appendNewLine("constraint('C" + std::to_string(n) + "_" + std::to_string(delay_count) + "', { 'b1', 's0' }, { 's0', 'b1' })");

    if (m_structure.cantorType() == CantorType::Linear_Cantor) {
        m_filePrinter.appendNewLine("grid('C" + std::to_string(n) + "_" + std::to_string(delay_count) + "', { { { 'b0' }, { 'b1' } } })");
    } else if (m_structure.cantorType() == CantorType::Quadratic_Cantor) {
        m_filePrinter.appendNewLine("grid('C" + std::to_string(n) + "_" + std::to_string(delay_count) + "', { { { 'b0' }, { 'intern_0' }, { 'b1' } } })");
    } else {
        //cubic
        m_filePrinter.appendNewLine("grid('C" + std::to_string(n) + "_" + std::to_string(delay_count) + "', { { { 'b0' }, { 'intern_0' }, { 'intern_1' }, { 'b1' } } })");
    }

    //matrices for intern points
    if (m_structure.cantorType() == CantorType::Cubic_Cantor) {
        m_filePrinter.appendNewLine("initMat('C" + std::to_string(n) + "_" + std::to_string(delay_count) + "', 's0', {");
        m_filePrinter.appendNewLine("    { 1, 0, 0, 0 },");
        m_filePrinter.appendNewLine("    { 0, 1, 0, 0 },");
        m_filePrinter.appendNewLine("    { 0, 0, 1, 0 },");
        m_filePrinter.appendNewLine("    { 0, 0, 0, 1 }");
        m_filePrinter.appendNewLine("}, 'CONST')");
    } else if (m_structure.cantorType() == CantorType::Quadratic_Cantor) {
        m_filePrinter.appendNewLine("initMat('C" + std::to_string(n) + "_" + std::to_string(delay_count) + "', 's0', {");
        m_filePrinter.appendNewLine("    { 1, 0, 0 },");
        m_filePrinter.appendNewLine("    { 0, 1, 0 },");
        m_filePrinter.appendNewLine("    { 0, 0, 1 },");
        m_filePrinter.appendNewLine("}, 'CONST')");
    }
}

void StructurePrinter::printCantorImpl(unsigned int n) {
    for (unsigned int i = 0; i < n; ++i) {
        m_filePrinter.appendNewLine("subdivision('s" + std::to_string(i) + "', 'C" + std::to_string(n) + "', 'C" + std::to_string(n) + "')");
    }

    if (m_structure.cantorType() == CantorType::Linear_Cantor) {
        m_filePrinter.appendNewLine("space('C" + std::to_string(n) + "', { 'b0', 'b1' })");
    } else if (m_structure.cantorType() == CantorType::Quadratic_Cantor) {
        m_filePrinter.appendNewLine("space('C" + std::to_string(n) + "', { 'b0', 'intern_0', 'b1' })");
    } else {
        //cubic
        m_filePrinter.appendNewLine("space('C" + std::to_string(n) + "', { 'b0', 'intern_0', 'intern_1', 'b1' })");
    }

    m_filePrinter.appendNewLine("constraint('C" + std::to_string(n) + "', { 'permut', 'b0' }, { 'b1' })");
    m_filePrinter.appendNewLine("constraint('C" + std::to_string(n) + "', { 'permut', 'b1' }, { 'b0' })");
    for (unsigned int i = 0; i < n; ++i) {
        m_filePrinter.appendNewLine("constraint('C" + std::to_string(n) + "', { 'permut', 's" + std::to_string(i) + "' }, { 's" + std::to_string(n - i - 1) + "', 'permut' })");
    }

    //permut intern
    if (m_structure.cantorType() == CantorType::Quadratic_Cantor) {
        m_filePrinter.appendNewLine("constraint('C" + std::to_string(n) + "', { 'permut', 'intern_0' }, { 'intern_0' })");
    } else if (m_structure.cantorType() == CantorType::Cubic_Cantor) {
        m_filePrinter.appendNewLine("constraint('C" + std::to_string(n) + "', { 'permut', 'intern_0' }, { 'intern_1' })");
        m_filePrinter.appendNewLine("constraint('C" + std::to_string(n) + "', { 'permut', 'intern_1' }, { 'intern_0' })");
    }

    m_filePrinter.appendNewLine("constraint('C" + std::to_string(n) + "', { 'b0', 's0' }, { 's0', 'b0' })");
    m_filePrinter.appendNewLine("constraint('C" + std::to_string(n) + "', { 'b1', 's0' }, { 's" + std::to_string(n - 1) + "', 'b1' })");

    if (m_structure.cantorType() == CantorType::Linear_Cantor) {
        m_filePrinter.appendNewLine("grid('C" + std::to_string(n) + "', { { { 'b0' }, { 'b1' } } })");
    } else if (m_structure.cantorType() == CantorType::Quadratic_Cantor) {
        m_filePrinter.appendNewLine("grid('C" + std::to_string(n) + "', { { { 'b0' }, { 'intern_0' }, { 'b1' } } })");
    } else {
        //cubic
        m_filePrinter.appendNewLine("grid('C" + std::to_string(n) + "', { { { 'b0' }, { 'intern_0' }, { 'intern_1' }, { 'b1' } } })");
    }

    //matrices
    if (m_structure.cantorType() == CantorType::Linear_Cantor) {
        for (unsigned int i = 0; i < n; ++i) {
            // for each subdivision T0, T1, ... Tn-1
            std::vector<float> t = frac::utils::getCantorLinearTransformation(i, n);
            m_filePrinter.appendNewLine("initMat('C" + std::to_string(n) + "', 's" + std::to_string(i) + "', {");
            m_filePrinter.appendNewLine("    { " + frac::utils::toString(t[0]) + ", " + frac::utils::toString(t[1]) + " },");
            m_filePrinter.appendNewLine("    { " + frac::utils::toString(t[2]) + ", " + frac::utils::toString(t[3]) + " },");
            m_filePrinter.appendNewLine("}, 'CONST')");
        }
    } else if (m_structure.cantorType() == CantorType::Quadratic_Cantor) {
        for (unsigned int i = 0; i < n; ++i) {
            // for each subdivision T0, T1, ... Tn-1
            std::vector<float> t = frac::utils::getCantorQuadraticTransformation(i, n);
            m_filePrinter.appendNewLine("initMat('C" + std::to_string(n) + "', 's" + std::to_string(i) + "', {");
            m_filePrinter.appendNewLine("    { " + frac::utils::toString(t[0]) + ", " + frac::utils::toString(t[1]) + ", " + frac::utils::toString(t[2]) + " },");
            m_filePrinter.appendNewLine("    { " + frac::utils::toString(t[3]) + ", " + frac::utils::toString(t[4]) + ", " + frac::utils::toString(t[5]) + " },");
            m_filePrinter.appendNewLine("    { " + frac::utils::toString(t[6]) + ", " + frac::utils::toString(t[7]) + ", " + frac::utils::toString(t[8]) + " },");
            m_filePrinter.appendNewLine("}, 'CONST')");
        }
    } else {
        //cubic
        for (unsigned int i = 0; i < n; ++i) {
            // for each subdivision T0, T1, ... Tn-1
            std::vector<float> t = frac::utils::getCantorCubicTransformation(i, n);
            m_filePrinter.appendNewLine("initMat('C" + std::to_string(n) + "', 's" + std::to_string(i) + "', {");
            m_filePrinter.appendNewLine("    { " + frac::utils::toString(t[0]) + ", " + frac::utils::toString(t[1]) + ", " + frac::utils::toString(t[2]) + ", " + frac::utils::toString(t[3]) + " },");
            m_filePrinter.appendNewLine("    { " + frac::utils::toString(t[4]) + ", " + frac::utils::toString(t[5]) + ", " + frac::utils::toString(t[6]) + ", " + frac::utils::toString(t[7]) + " },");
            m_filePrinter.appendNewLine("    { " + frac::utils::toString(t[8]) + ", " + frac::utils::toString(t[9]) + ", " + frac::utils::toString(t[10]) + ", " + frac::utils::toString(t[11]) + " },");
            m_filePrinter.appendNewLine("    { " + frac::utils::toString(t[12]) + ", " + frac::utils::toString(t[13]) + ", " + frac::utils::toString(t[14]) + ", " + frac::utils::toString(t[15]) + " },");
            m_filePrinter.appendNewLine("}, 'CONST')");
        }
    }
}

void StructurePrinter::printDelayBezierImpl(unsigned int n, unsigned int delay_count) {
    m_filePrinter.appendNewLine("subdivision('s0', 'B" + std::to_string(n) + "_" + std::to_string(delay_count) + "', '" + (delay_count > 1 ? "B" + std::to_string(n) + "_" + std::to_string(delay_count - 1) : "B" + std::to_string(n)) + "')");
    if (m_structure.bezierType() == BezierType::Linear_Bezier) {
        m_filePrinter.appendNewLine("space('B" + std::to_string(n) + "_" + std::to_string(delay_count) + "', { 'b0', 'b1' })");
    } else if (m_structure.bezierType() == BezierType::Quadratic_Bezier) {
        m_filePrinter.appendNewLine("space('B" + std::to_string(n) + "_" + std::to_string(delay_count) + "', { 'b0', 'intern_0', 'b1' })");
    } else {
        //cubic
        m_filePrinter.appendNewLine("space('B" + std::to_string(n) + "_" + std::to_string(delay_count) + "', { 'b0', 'intern_0', 'intern_1', 'b1' })");
    }
    m_filePrinter.appendNewLine("constraint('B" + std::to_string(n) + "_" + std::to_string(delay_count) + "', { 'permut', 'b0' }, { 'b1' })");
    m_filePrinter.appendNewLine("constraint('B" + std::to_string(n) + "_" + std::to_string(delay_count) + "', { 'permut', 'b1' }, { 'b0' })");

    //permut intern
    if (m_structure.bezierType() == BezierType::Quadratic_Bezier) {
        m_filePrinter.appendNewLine("constraint('B" + std::to_string(n) + "_" + std::to_string(delay_count) + "', { 'permut', 'intern_0' }, { 'intern_0' })");
    } else if (m_structure.bezierType() == BezierType::Cubic_Bezier) {
        m_filePrinter.appendNewLine("constraint('B" + std::to_string(n) + "_" + std::to_string(delay_count) + "', { 'permut', 'intern_0' }, { 'intern_1' })");
        m_filePrinter.appendNewLine("constraint('B" + std::to_string(n) + "_" + std::to_string(delay_count) + "', { 'permut', 'intern_1' }, { 'intern_0' })");
    }

    m_filePrinter.appendNewLine("constraint('B" + std::to_string(n) + "_" + std::to_string(delay_count) + "', { 'permut', 's0' }, { 's0', 'permut' })");
    m_filePrinter.appendNewLine("constraint('B" + std::to_string(n) + "_" + std::to_string(delay_count) + "', { 'b0', 's0' }, { 's0', 'b0' })");
    m_filePrinter.appendNewLine("constraint('B" + std::to_string(n) + "_" + std::to_string(delay_count) + "', { 'b1', 's0' }, { 's0', 'b1' })");

    if (m_structure.bezierType() == BezierType::Linear_Bezier) {
        m_filePrinter.appendNewLine("grid('B" + std::to_string(n) + "_" + std::to_string(delay_count) + "', { { { 'b0' }, { 'b1' } } })");
    } else if (m_structure.bezierType() == BezierType::Quadratic_Bezier) {
        m_filePrinter.appendNewLine("grid('B" + std::to_string(n) + "_" + std::to_string(delay_count) + "', { { { 'b0' }, { 'intern_0' }, { 'b1' } } })");
    } else {
        //cubic
        m_filePrinter.appendNewLine("grid('B" + std::to_string(n) + "_" + std::to_string(delay_count) + "', { { { 'b0' }, { 'intern_0' }, { 'intern_1' }, { 'b1' } } })");
    }

    //matrices for intern points
    if (m_structure.bezierType() == BezierType::Cubic_Bezier) {
        m_filePrinter.appendNewLine("initMat('B" + std::to_string(n) + "_" + std::to_string(delay_count) + "', 's0', {");
        m_filePrinter.appendNewLine("    { 1, 0, 0, 0 },");
        m_filePrinter.appendNewLine("    { 0, 1, 0, 0 },");
        m_filePrinter.appendNewLine("    { 0, 0, 1, 0 },");
        m_filePrinter.appendNewLine("    { 0, 0, 0, 1 }");
        m_filePrinter.appendNewLine("}, 'CONST')");
    } else if (m_structure.bezierType() == BezierType::Quadratic_Bezier) {
        m_filePrinter.appendNewLine("initMat('B" + std::to_string(n) + "_" + std::to_string(delay_count) + "', 's0', {");
        m_filePrinter.appendNewLine("    { 1, 0, 0 },");
        m_filePrinter.appendNewLine("    { 0, 1, 0 },");
        m_filePrinter.appendNewLine("    { 0, 0, 1 },");
        m_filePrinter.appendNewLine("}, 'CONST')");
    }
}

void StructurePrinter::printBezierImpl(unsigned int n) {
    for (unsigned int i = 0; i < n; ++i) {
        m_filePrinter.appendNewLine("subdivision('s" + std::to_string(i) + "', 'B" + std::to_string(n) + "', 'B" + std::to_string(n) + "')");
    }

    if (m_structure.bezierType() == BezierType::Linear_Bezier) {
        m_filePrinter.appendNewLine("space('B" + std::to_string(n) + "', { 'b0', 'b1' })");
    } else if (m_structure.bezierType() == BezierType::Quadratic_Bezier) {
        m_filePrinter.appendNewLine("space('B" + std::to_string(n) + "', { 'b0', 'intern_0', 'b1' })");
    } else {
        //cubic
        m_filePrinter.appendNewLine("space('B" + std::to_string(n) + "', { 'b0', 'intern_0', 'intern_1', 'b1' })");
    }

    m_filePrinter.appendNewLine("constraint('B" + std::to_string(n) + "', { 'permut', 'b0' }, { 'b1' })");
    m_filePrinter.appendNewLine("constraint('B" + std::to_string(n) + "', { 'permut', 'b1' }, { 'b0' })");
    for (unsigned int i = 0; i < n; ++i) {
        m_filePrinter.appendNewLine("constraint('B" + std::to_string(n) + "', { 'permut', 's" + std::to_string(i) + "' }, { 's" + std::to_string(n - i - 1) + "', 'permut' })");
    }

    //permut intern
    if (m_structure.bezierType() == BezierType::Quadratic_Bezier) {
        m_filePrinter.appendNewLine("constraint('B" + std::to_string(n) + "', { 'permut', 'intern_0' }, { 'intern_0' })");
    } else if (m_structure.bezierType() == BezierType::Cubic_Bezier) {
        m_filePrinter.appendNewLine("constraint('B" + std::to_string(n) + "', { 'permut', 'intern_0' }, { 'intern_1' })");
        m_filePrinter.appendNewLine("constraint('B" + std::to_string(n) + "', { 'permut', 'intern_1' }, { 'intern_0' })");
    }

    m_filePrinter.appendNewLine("constraint('B" + std::to_string(n) + "', { 'b0', 's0' }, { 's0', 'b0' })");
    m_filePrinter.appendNewLine("constraint('B" + std::to_string(n) + "', { 'b1', 's0' }, { 's" + std::to_string(n - 1) + "', 'b1' })");

    if (m_structure.bezierType() == BezierType::Linear_Bezier) {
        m_filePrinter.appendNewLine("grid('B" + std::to_string(n) + "', { { { 'b0' }, { 'b1' } } })");
    } else if (m_structure.bezierType() == BezierType::Quadratic_Bezier) {
        m_filePrinter.appendNewLine("grid('B" + std::to_string(n) + "', { { { 'b0' }, { 'intern_0' }, { 'b1' } } })");
    } else {
        //cubic
        m_filePrinter.appendNewLine("grid('B" + std::to_string(n) + "', { { { 'b0' }, { 'intern_0' }, { 'intern_1' }, { 'b1' } } })");
    }

    //matrices
    if (m_structure.bezierType() == BezierType::Linear_Bezier) {
        for (unsigned int i = 0; i < n; ++i) {
            // for each subdivision T0, T1, ... Tn-1
            std::vector<float> t = frac::utils::getBezierLinearTransformation(i, n);
            m_filePrinter.appendNewLine("initMat('B" + std::to_string(n) + "', 's" + std::to_string(i) + "', {");
            m_filePrinter.appendNewLine("    { " + frac::utils::toString(t[0]) + ", " + frac::utils::toString(t[1]) + " },");
            m_filePrinter.appendNewLine("    { " + frac::utils::toString(t[2]) + ", " + frac::utils::toString(t[3]) + " },");
            m_filePrinter.appendNewLine("}, 'CONST')");
        }
    } else if (m_structure.bezierType() == BezierType::Quadratic_Bezier) {
        for (unsigned int i = 0; i < n; ++i) {
            // for each subdivision T0, T1, ... Tn-1
            std::vector<float> t = frac::utils::getBezierQuadraticTransformation(i, n);
            m_filePrinter.appendNewLine("initMat('B" + std::to_string(n) + "', 's" + std::to_string(i) + "', {");
            m_filePrinter.appendNewLine("    { " + frac::utils::toString(t[0]) + ", " + frac::utils::toString(t[1]) + ", " + frac::utils::toString(t[2]) + " },");
            m_filePrinter.appendNewLine("    { " + frac::utils::toString(t[3]) + ", " + frac::utils::toString(t[4]) + ", " + frac::utils::toString(t[5]) + " },");
            m_filePrinter.appendNewLine("    { " + frac::utils::toString(t[6]) + ", " + frac::utils::toString(t[7]) + ", " + frac::utils::toString(t[8]) + " },");
            m_filePrinter.appendNewLine("}, 'CONST')");
        }
    } else {
        //cubic
        for (unsigned int i = 0; i < n; ++i) {
            // for each subdivision T0, T1, ... Tn-1
            std::vector<float> t = frac::utils::getBezierCubicTransformation(i, n);
            m_filePrinter.appendNewLine("initMat('B" + std::to_string(n) + "', 's" + std::to_string(i) + "', {");
            m_filePrinter.appendNewLine("    { " + frac::utils::toString(t[0]) + ", " + frac::utils::toString(t[1]) + ", " + frac::utils::toString(t[2]) + ", " + frac::utils::toString(t[3]) + " },");
            m_filePrinter.appendNewLine("    { " + frac::utils::toString(t[4]) + ", " + frac::utils::toString(t[5]) + ", " + frac::utils::toString(t[6]) + ", " + frac::utils::toString(t[7]) + " },");
            m_filePrinter.appendNewLine("    { " + frac::utils::toString(t[8]) + ", " + frac::utils::toString(t[9]) + ", " + frac::utils::toString(t[10]) + ", " + frac::utils::toString(t[11]) + " },");
            m_filePrinter.appendNewLine("    { " + frac::utils::toString(t[12]) + ", " + frac::utils::toString(t[13]) + ", " + frac::utils::toString(t[14]) + ", " + frac::utils::toString(t[15]) + " },");
            m_filePrinter.appendNewLine("}, 'CONST')");
        }
    }
}

void StructurePrinter::printInitSubds() {
    auto const& subds = m_structure.faces();
    int i = 0;
    for (auto const& s : subds) {
        m_filePrinter.appendNewLine("subdivision('s" + std::to_string(i) + "', 'init', '" + s.name() + "')");
        i += 1;
    }
}

void StructurePrinter::printEdgesOfCell(Face const& cell) {
    int i = 0;
    for (auto const& edge : cell.constData()) {
        m_filePrinter.appendNewLine("boundary('b" + std::to_string(i) + "', '" + cell.name() + "', '" + edge.name() + "')");
        i += 1;
    }
}

void StructurePrinter::printSubdOfCell(Face const& cell) {
    std::vector<Face> subds = cell.subdivisions();
    int i = 0;
    for (Face const& f : subds) {
        m_filePrinter.appendNewLine("subdivision('s" + std::to_string(i) + "', '" + cell.name() + "', '" + f.name() + "')");
        i += 1;
    }
}

void StructurePrinter::printSpaceOfCell(Face const& cell) {
    m_filePrinter.append("space('" + cell.name() + "', { ");
    for (std::size_t i = 0; i < cell.len(); ++i) {
        if (i == 0) {
            m_filePrinter.append("'b" + std::to_string(i) + "'");
        } else {
            m_filePrinter.append(", 'b" + std::to_string(i) + "'");
        }
    }
    m_filePrinter.appendNewLine("})");
}

void StructurePrinter::printPrimOfCell(Face const& cell) {
    m_filePrinter.appendNewLine("primitive('" + cell.name() + "', { {");
    for (std::size_t i = 0; i < cell.len(); ++i) {
        if (cell[i].edgeType() == EdgeType::BEZIER && cell[i].delay() == 0) {
            for (std::size_t j = 0; j < cell[i].nbSubdivisions(); ++j) {
                if (cell[i].nbSubdivisions() > 2) {
                    m_filePrinter.appendNewLine("    { 'b" + std::to_string(i) + "', 's" + std::to_string(j) + "', 'b0' },");
                } else {
                    for (std::size_t k = 0; k < cell[i].nbSubdivisions(); ++k) {
                        m_filePrinter.appendNewLine("    { 'b" + std::to_string(i) + "', 's" + std::to_string(j) + "', 's" + std::to_string(k) + "', 'b0' },");
                    }
                }
            }
        } else {
            m_filePrinter.appendNewLine("    { 'b" + std::to_string(i) + "', 'b0' },");
        }
    }
    m_filePrinter.appendNewLine("} })");
}

void StructurePrinter::printEdgeAdjacenciesOfCell(Face const& cell) {
    for (std::size_t i = 0; i < cell.len(); ++i) {
        m_filePrinter.appendNewLine("constraint('" + cell.name() + "', { 'b" + std::to_string(i) + "', 'b1' }, { 'b" + std::to_string(utils::mod(i + 1, cell.len())) + "', 'b0' })");
    }
}

} // frac
