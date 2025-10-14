#ifndef AUTOFRAC_LAYERBCIFS_H
#define AUTOFRAC_LAYERBCIFS_H

#include "layer.h"

class LayerBcifs : public Layer {
public:
    LayerBcifs();

    static void testConstraints();
    static void testBCIFSAutomaton();
};


#endif //AUTOFRAC_LAYERBCIFS_H
