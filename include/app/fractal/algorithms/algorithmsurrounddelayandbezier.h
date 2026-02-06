#ifndef AUTOFRAC_ALGORITHMSURROUNDDELAYANDBEZIER_H
#define AUTOFRAC_ALGORITHMSURROUNDDELAYANDBEZIER_H

#include <vector>

namespace frac {

class Face;

namespace LinksSurroundDelayAndBezier {

    std::vector<Face> subdivide(Face const& face);

} // LinksSurroundDelayAndBezier

} // frac

#endif //AUTOFRAC_ALGORITHMSURROUNDDELAYANDBEZIER_H
