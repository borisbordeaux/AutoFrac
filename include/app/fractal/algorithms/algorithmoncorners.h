#ifndef AUTOFRAC_ALGORITHMONCORNERS_H
#define AUTOFRAC_ALGORITHMONCORNERS_H

#include <vector>

namespace frac {

class Face;

namespace LinksOnCorners {

    std::vector<Face> subdivide(const Face& face);

} // LinksOnCorners

} // frac

#endif //AUTOFRAC_ALGORITHMONCORNERS_H
