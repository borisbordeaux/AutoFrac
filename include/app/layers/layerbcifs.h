#ifndef AUTOFRAC_LAYERBCIFS_H
#define AUTOFRAC_LAYERBCIFS_H

#include "app/batch/batchcontrolpoint.h"
#include "app/batch/batchface.h"
#include "app/batch/batchgrid.h"
#include "app/batch/batchsubdivisionpoint.h"
#include "app/bcifs/bcifs.h"
#include "app/bcifs/subdivisionpoint.h"
#include "core/camera.h"
#include "core/layer.h"

class LayerEditFractal;

namespace Core {
class Event;
class KeyPressedEvent;
class KeyReleasedEvent;
class LayerSwappedEvent;
class MouseButtonPressedEvent;
class MouseButtonReleasedEvent;
class MouseMovedEvent;
class MouseScrolledEvent;
class WindowResizedEvent;
}

class LayerBcifs : public Core::Layer {
public:
    LayerBcifs(LayerEditFractal* layerEditFractal);
    void testSubdQuad();
    void testSierpinski();
    void testG2(int rows, int cols);
    void testSquareSierpinski();
    void onUpdate(float deltaTime) override;
    void onRender() override;
    void onImGuiRender() override;
    void onEvent(Core::Event& event) override;

private:
    bool onMousePressedEvent(const Core::MouseButtonPressedEvent& event);
    bool onMouseMovedEvent(const Core::MouseMovedEvent& event);
    bool onMouseReleasedEvent(const Core::MouseButtonReleasedEvent& event);
    bool onWindowResizedEvent(const Core::WindowResizedEvent& event);
    bool onMouseScrolledEvent(const Core::MouseScrolledEvent& event);
    bool onKeyPressedEvent(const Core::KeyPressedEvent& event);
    bool onKeyReleasedEvent(const Core::KeyReleasedEvent& event);
    bool onLayerSwappedEvent(const Core::LayerSwappedEvent& event);
    void handleSelection();
    void handleMoveControlPoint();
    void handleMoveSubdivisionPoint();
    static bool intersectRayPlane(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, const glm::vec3& planePoint, const glm::vec3& planeNormal, float& t);
    void loadLuaFile(const std::string& filename);
    void loadLuaScript(const std::string& luaScript);

private:
    BCIFS::Bcifs m_bcifs;
    int m_iterationLevel = 0;
    bool m_updateMSSSubdivisionPoints = false;
    bool m_updateMSSControlPoints = false;
    int m_nbIterationsMSS = 1000;
    int m_currentIterationMSS = m_nbIterationsMSS;
    bool m_bcifsChanged = false;
    bool m_gridChanged = false;
    bool m_displayGrid = false;
    bool m_displayHidden = true;
    int m_gridLevel = 0;
    BatchFace m_batchFace;
    BatchGrid m_batchGrid;
    BatchControlPoint m_batchControlPoint;
    BatchSubdivisionPoint m_batchSubdivisionPoint;
    bool m_leftMousePressed = false;
    bool m_rightMousePressed = false;
    bool m_uniformsDirty = true;
    glm::vec<2, double> m_mousePos;
    Core::Camera m_camera;
    glm::mat4 m_proj;
    std::optional<BCIFS::FormalMatrix> m_currentControlPoint;
    std::optional<BCIFS::SubdivisionPoint> m_currentSubdivisionPoint;
    glm::vec3 m_initialControlPointPosition;
    bool m_xKeyPressed = false;
    bool m_yKeyPressed = false;
    bool m_zKeyPressed = false;
    int m_colorDepth = 0;
    int m_currentIlluminationItem = 0;
    IlluminationMode m_illuminationMode = IlluminationMode::PHONG;
    glm::vec3 m_clearColor = glm::vec3(1.0f, 1.0f, 1.0f);
    bool m_clearColorChanged = true;
    LayerEditFractal* m_layerEditFractal;
};

#endif //AUTOFRAC_LAYERBCIFS_H
