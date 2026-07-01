#ifndef AUTOFRAC_AUTOFRAC_JSONEXPORTER_H
#define AUTOFRAC_AUTOFRAC_JSONEXPORTER_H

#include <string>
#include <glm/vec3.hpp>

#include "app/bcifs/bcifs.h"
#include "app/bcifs/automaton.h"
#include "app/bcifs/transition.h"

namespace BCIFS {
class Bcifs;
}

class JSONExporter {
public:
    static void exportJson(const std::string& filename, BCIFS::Bcifs& bcifs);
};

#endif //AUTOFRAC_JSONEXPORTER_H