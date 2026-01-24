#ifndef AUTOFRAC_ALGORITHMSURROUNDDELAY_H
#define AUTOFRAC_ALGORITHMSURROUNDDELAY_H

#include <vector>
#include "app/fractal/face.h"

namespace frac::LinksSurroundDelay {

std::vector<frac::Face> subdivide(frac::Face const& face);

}

#endif //AUTOFRAC_ALGORITHMSURROUNDDELAY_H
