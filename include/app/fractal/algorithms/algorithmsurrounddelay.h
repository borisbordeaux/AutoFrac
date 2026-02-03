#ifndef AUTOFRAC_ALGORITHMSURROUNDDELAY_H
#define AUTOFRAC_ALGORITHMSURROUNDDELAY_H

#include <vector>

namespace frac {

class Face;

namespace LinksSurroundDelay {

    std::vector<Face> subdivide(Face const& face);

} // LinksSurroundDelay

} // frac

#endif //AUTOFRAC_ALGORITHMSURROUNDDELAY_H
