#ifndef AUTOFRAC_LAYERTEXTURE_H
#define AUTOFRAC_LAYERTEXTURE_H

#include <glm/detail/type_vec3.hpp>
#include <glm/detail/type_vec2.hpp>
#include "core/layer.h"
#include "core/vertexarray.h"
#include "core/vertexbuffer.h"
#include "core/vertexbufferlayout.h"
#include "core/indexbuffer.h"
#include "core/shader.h"
#include "core/texture.h"
#include "core/renderer.h"

namespace Core {
class MouseButtonPressedEvent;

class MouseMovedEvent;

class MouseButtonReleasedEvent;

class Event;
}

class LayerTexture : public Core::Layer {
public:
    LayerTexture();
    void onUpdate(float deltaTime) override;
    void onRender() override;
    void onImGuiRender() override;
    void onEvent(Core::Event& event) override;

private:
    bool onMousePressedEvent(Core::MouseButtonPressedEvent& event);
    bool onMouseMovedEvent(Core::MouseMovedEvent& event);
    bool onMouseReleasedEvent(Core::MouseButtonReleasedEvent& event);

private:
    Core::VertexArray m_vao;
    Core::VertexBuffer m_vbo;
    Core::VertexBufferLayout m_layout;
    Core::IndexBuffer m_ibo;
    Core::ShaderProgram m_program;
    Core::Texture m_texture;

    float m_blue = 1.0f;
    float m_increment = 0.01f;
    glm::vec3 m_translation = { 0.0f, 0.0f, 0.0f };
    glm::vec3 m_cameraPos = { 0.0f, 0.0f, 5.0f };

    bool m_mousePressed = false;
    glm::vec2 m_mousePos;
};

#endif //AUTOFRAC_LAYERTEXTURE_H
