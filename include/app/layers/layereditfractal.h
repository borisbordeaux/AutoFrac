#ifndef AUTOFRAC_LAYEREDITFRACTAL_H
#define AUTOFRAC_LAYEREDITFRACTAL_H

#include "core/layer.h"
#include <string>
#include <vector>

#include "app/fractal/edge.h"
#include "app/fractal/face.h"

namespace Core {
class LayerSwappedEvent;
}

class LayerEditFractal : public Core::Layer {
public:
    void onImGuiRender() override;
    void onEvent(Core::Event&) override;
    const std::vector<std::string>& faces() const { return m_faces; }
    int cantorType() const { return m_cantorType; }
    int bezierType() const { return m_bezierType; }
    bool edited() const { return m_edited; }

private:
    void updateEdited();
    bool onLayerSwappedEvent(const Core::LayerSwappedEvent& event);
    static bool mySliderUnsignedInt(const char *label, unsigned int* value, unsigned int min = 0);
    void displayEdgeSettings(const std::string& desc, frac::Edge& edge, int* type);

private:
    // faces
    std::vector<std::string> m_faces;
    int m_EaType = 0;
    int m_ElType = 1;
    int m_EcType = 1;
    int m_faceDelay = 0;
    int m_faceProc = 1;
    std::size_t m_selectedFace = 0;
    frac::Face m_currentFace;
    // edges
    int m_selectedEdgeType = 0;
    std::size_t m_selectedEdge = 0;
    // geometry
    int m_cantorType = 0;
    int m_bezierType = 1;
    // to avoid construction of new structure if no changes
    bool m_edited = false;
    std::vector<std::string> m_lastFaces;
    int m_lastCantorType = 0;
    int m_lastBezierType = 1;
};

#endif //AUTOFRAC_LAYEREDITFRACTAL_H
