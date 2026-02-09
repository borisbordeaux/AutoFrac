#ifndef AUTOFRAC_LAYEREDITFRACTAL_H
#define AUTOFRAC_LAYEREDITFRACTAL_H

#include "core/layer.h"
#include <string>
#include <vector>

namespace Core {
class LayerSwappedEvent;
}

class LayerEditFractal : public Core::Layer {
public:
    void onImGuiRender() override;
    void onEvent(Core::Event&) override;
    std::string face() const { return m_face; }
    int cantorType() const { return m_cantorType; }
    int bezierType() const { return m_bezierType; }
    bool edited() const { return m_edited; }

private:
    static std::string buildEdge(int type, int nbSubs, int delay);
    void buildFace();
    bool onLayerSwappedEvent(const Core::LayerSwappedEvent& event);

private:
    int m_currentEdgeType = 0;
    int m_currentEdgeNbSubs = 2;
    int m_currentEdgeDelay = 0;
    int m_faceDelay = 0;
    int m_faceProc = 1;
    std::vector<std::string> m_edges;
    std::string m_face = "";
    bool m_edited = true;
    std::size_t m_currentEdge = 0;
    std::string m_lastFace = "";
    int m_cantorType = 0;
    int m_bezierType = 1;
    int m_lastCantorType = 0;
    int m_lastBezierType = 1;
    int m_EaType = 0;
    int m_EaNbSubs = 2;
    int m_EaDelay = 0;
    int m_ElType = 1;
    int m_ElNbSubs = 2;
    int m_ElDelay = 0;
    int m_EcType = 1;
    int m_EcNbSubs = 2;
    int m_EcDelay = 0;
};

#endif //AUTOFRAC_LAYEREDITFRACTAL_H
