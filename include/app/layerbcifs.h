#ifndef AUTOFRAC_LAYERBCIFS_H
#define AUTOFRAC_LAYERBCIFS_H

#include "core/layer.h"

class LayerBcifs : public Core::Layer {
public:
    LayerBcifs();

    static void testConstraints();
    static void testBCIFSAutomaton();
};


#endif //AUTOFRAC_LAYERBCIFS_H
