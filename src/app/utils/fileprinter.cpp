#include "app/utils/fileprinter.h"
#include <fstream>
#include <ostream>

void frac::FilePrinter::append(std::string const& text) {
    m_output += text;
}

void frac::FilePrinter::append_nl(std::string const& text) {
    m_output += text + '\n';
}

void frac::FilePrinter::printToFile(std::string const& filename) const {
    std::ofstream file;
    file.open(filename, std::ofstream::out | std::ofstream::trunc);
    file << m_output;
    file.close();
}

void frac::FilePrinter::reset() {
    m_output = "";
}
