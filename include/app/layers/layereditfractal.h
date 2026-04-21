#ifndef AUTOFRAC_LAYEREDITFRACTAL_H
#define AUTOFRAC_LAYEREDITFRACTAL_H

#include "core/layer.h"

#include <string>
#include <vector>

#include "app/batch/batchcontrolpointscheme.h"
#include "app/batch/batchedgesscheme.h"
#include "app/batch/batchfacescheme.h"
#include "app/fractal/face.h"
#include "app/fractal/structure.h"
#include "core/camera.h"

namespace Core {
class MouseScrolledEvent;
class MouseButtonReleasedEvent;
class MouseMovedEvent;
class WindowResizedEvent;
class MouseButtonPressedEvent;
class LayerSwappedEvent;
}

class LayerEditFractal : public Core::Layer {
public:
    LayerEditFractal();
    void onUpdate(float deltaTime) override;
    void onImGuiRender() override;
    void onEvent(Core::Event& event) override;
    void onRender() override;
    const std::vector<std::string>& faces() const { return m_faces; }
    int cantorType() const { return m_cantorType; }
    int bezierType() const { return m_bezierType; }
    bool edited() const { return m_edited; }
    const frac::Structure& structure() const { return m_structure; }
    void setCamera(Core::Camera* camera);

private:
    bool onMousePressedEvent(const Core::MouseButtonPressedEvent& event);
    bool onMouseMovedEvent(const Core::MouseMovedEvent& event);
    bool onMouseReleasedEvent(const Core::MouseButtonReleasedEvent& event);
    bool onMouseScrolledEvent(const Core::MouseScrolledEvent& event);
    bool onWindowResizedEvent(const Core::WindowResizedEvent& event);
    void handleSelection();
    void handleMoveControlPoint();
    static bool intersectRayPlane(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, const glm::vec3& planePoint, const glm::vec3& planeNormal, float& t);

    void updateEdited();
    bool onLayerSwappedEvent(const Core::LayerSwappedEvent& event);
    static bool mySliderUnsignedInt(const char* label, unsigned int* value, unsigned int min = 0);
    void displayEdgeSettings(const std::string& desc, frac::Edge& edge, int* type);

    void updateStructure(bool changeControlPoints = true);
    void updateBatchFace();
    void updateBatchControlPoints();
    void updateBatchEdge();

private:
    // faces
    std::vector<std::string> m_faces;
    int m_EaType = 0;
    int m_ElType = 1;
    int m_EcType = 1;
    int m_faceDelay = 0;
    int m_faceProc = 1;
    std::size_t m_selectedFace = 0;
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
    // colors
    int m_colorType = 1;
    int m_lastColorType = 1;
    // fractal stucture
    frac::Structure m_structure;
    // rendering
    bool m_uniformsDirty = true;
    Core::Camera* m_camera = nullptr;
    glm::mat4 m_proj;
    BatchFaceScheme m_batchFaceScheme;
    BatchControlPointScheme m_batchControlPointScheme;
    BatchEdgesScheme m_batchEdgesScheme;
    // moving control points
    bool m_leftMousePressed = false;
    bool m_rightMousePressed = false;
    glm::vec<2, double> m_mousePos;
    std::optional<std::pair<std::size_t, std::size_t>> m_currentControlPoint; // index of face and index of control point
    bool m_movedControlPoints = false;
};

#endif //AUTOFRAC_LAYEREDITFRACTAL_H
