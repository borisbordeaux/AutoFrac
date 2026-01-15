#ifndef AUTOFRAC_LAYERBCIFS_H
#define AUTOFRAC_LAYERBCIFS_H

#include "app/batch/batchcontrolpoint.h"
#include "app/batch/batchface.h"
#include "app/batch/batchgrid.h"
#include "app/batch/batchsubdivisionpoint.h"
#include "core/layer.h"
#include "app/bcifs/bcifs.h"
#include "core/camera.h"

namespace Core {
class MouseButtonPressedEvent;

class MouseMovedEvent;

class MouseButtonReleasedEvent;

class MouseScrolledEvent;

class WindowResizedEvent;

class KeyPressedEvent;

class KeyReleasedEvent;

class Event;
}

class LayerBcifs : public Core::Layer {
public:
    LayerBcifs();

    static void testConstraints();
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

    void handleSelection();
    void handleMoveControlPoint();
    void handleMoveSubdivisionPoint();
    static bool intersectRayPlane(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, const glm::vec3& planePoint, const glm::vec3& planeNormal, float& t);

    void loadLuaFile();

private:
    BCIFS::Bcifs m_bcifs;
    int m_iterationLevel = 0;
    bool m_updateMSS = false;
    int m_nbIterationsMSS = 500;
    int m_currentIterationMSS = m_nbIterationsMSS;
    bool m_bcifsChanged = false;
    bool m_displayGrid = false;

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
};


#endif //AUTOFRAC_LAYERBCIFS_H
