#ifndef AUTOFRAC_STLEXPORTER_H
#define AUTOFRAC_STLEXPORTER_H

#include <string>
#include <glm/vec3.hpp>

namespace BCIFS {
class Bcifs;
}

class STLExporter {
public:
    static void exportBinary(const std::string& filename, BCIFS::Bcifs& bcifs, std::size_t iterationLevel);

private:
    static void writeVec3(std::ofstream& file, const glm::vec3& v);
};

#endif //AUTOFRAC_STLEXPORTER_H