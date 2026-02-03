#ifndef AUTOFRAC_UTILS_H
#define AUTOFRAC_UTILS_H

#include <algorithm>
#include <iomanip>
#include <ios>
#include <sstream>
#include <string>

namespace BCIFS::utils {

static std::string toString(float value) {
    std::stringstream stream;
    stream << std::fixed << std::setprecision(4) << value;
    std::string res = stream.str();
    std::replace(res.begin(), res.end(), ',', '.');
    return res;
}

}

#endif //AUTOFRAC_UTILS_H
