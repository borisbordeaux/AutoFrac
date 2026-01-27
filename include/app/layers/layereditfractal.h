#ifndef AUTOFRAC_LAYEREDITFRACTAL_H
#define AUTOFRAC_LAYEREDITFRACTAL_H

#include "core/layer.h"
#include <string>
#include <vector>

class LayerEditFractal : public Core::Layer {
public:
    void onImGuiRender() override;
    std::string face() const { return m_currentFace; }

private:
    std::string buildEdge() const;
    void buildFace();

private:
    int m_currentEdgeType = 0;
    int m_currentEdgeNbSubs = 2;
    int m_currentEdgeDelay = 0;
    int m_currentFaceDelay = 0;
    int m_currentFaceAlgo = 0;
    std::vector<std::string> m_edges;
    std::string m_currentFace = "";
};

#endif //AUTOFRAC_LAYEREDITFRACTAL_H
