#ifndef AUTOFRAC_LAYERBCIFS_H
#define AUTOFRAC_LAYERBCIFS_H

#include "core/layer.h"
#include "app/bcifs/bcifs.h"
#include "core/vertexarray.h"
#include "core/vertexbuffer.h"
#include "core/vertexbufferlayout.h"
#include "core/shader.h"
#include "core/camera.h"

namespace Core {
class MouseButtonPressedEvent;

class MouseMovedEvent;

class MouseButtonReleasedEvent;

class MouseScrolledEvent;

class WindowResizedEvent;

class Event;
}

class LayerBcifs : public Core::Layer {
public:
    LayerBcifs();

    void testConstraints();
    void testBCIFSAutomaton();

    void onUpdate(float deltaTime) override;
    void onRender() override;
    void onImGuiRender() override;
    void onEvent(Core::Event& event) override;

private:
    bool onMousePressedEvent(Core::MouseButtonPressedEvent& event);
    bool onMouseMovedEvent(Core::MouseMovedEvent& event);
    bool onMouseReleasedEvent(Core::MouseButtonReleasedEvent& event);
    bool onWindowResizedEvent(Core::WindowResizedEvent& event);
    bool onMouseScrolledEvent(Core::MouseScrolledEvent& event);

private:
    static std::size_t findNbTriangles(const std::vector<std::vector<glm::vec3>>& faces);
    void addFace(const std::vector<glm::vec3>& vertices);
    void addTriangle(const glm::vec3& pos1, const glm::vec3& pos2, const glm::vec3& pos3);
    void addVertexFace(const glm::vec3& v, const glm::vec3& n);

private:
    BCIFS::Bcifs m_bcifs;
    int m_iterationLevel = 0;

    Core::VertexArray m_vao;
    Core::VertexBuffer m_vbo;
    Core::VertexBufferLayout m_layout;
    Core::ShaderProgram m_program;

    std::vector<float> m_data;
    int m_count = 0;
    int m_floatsPerVertex = 6;
    bool m_bcifsChanged = false;

    bool m_leftMousePressed = false;
    bool m_rightMousePressed = false;
    bool m_uniformsDirty = true;
    glm::vec<2, double, glm::defaultp> m_mousePos;
    Core::Camera m_camera;
    glm::mat4 m_proj;
};


#endif //AUTOFRAC_LAYERBCIFS_H
