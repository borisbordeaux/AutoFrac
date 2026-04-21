#ifndef AUTOFRAC_STRUCTUREPRINTER_H
#define AUTOFRAC_STRUCTUREPRINTER_H

#include <string>

#include "app/utils/fileprinter.h"
#include "app/utils/set.h"

namespace frac {

class Edge;
class Face;
class Structure;

class StructurePrinter {
public:
    explicit StructurePrinter(Structure const& structure, std::string filename = "");
    std::string exportStruct();

private:
    void printVertexState();
    void printDeclOfEdge(Edge const& edge);
    void printDelayCantorDecl(unsigned int n, unsigned int delay_count);
    void printCantorDecl(unsigned int n);
    void printDelayBezierDecl(unsigned int n, unsigned int delay_count);
    void printBezierDecl(unsigned int n);
    void printImplOfEdge(Edge const& edge);
    void printDelayCantorImpl(unsigned int n, unsigned int delay_count);
    void printCantorImpl(unsigned int n);
    void printDelayBezierImpl(unsigned int n, unsigned int delay_count);
    void printBezierImpl(unsigned int n);
    void printInitSubds(Set<Face> const& allCells);
    void printEdgesOfCell(Face const& cell);
    void printSubdOfCell(Face const& cell, Set<Face> const& allCells);
    void printSpaceOfCell(Face const& cell);
    void printPrimOfCell(Face const& cell);
    void printEdgeAdjacenciesOfCell(Face const& cell);
    void printControlPoints();

private:
    Structure const& m_structure;
    std::string const m_filename;
    FilePrinter m_filePrinter;
};

} // frac

#endif //AUTOFRAC_STRUCTUREPRINTER_H
