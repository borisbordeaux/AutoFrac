#include "app/layers/layertexture.h"

#include <glm/detail/type_mat4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <GLFW/glfw3.h>

#include "imgui/imgui.h"
#include "core/event.h"
#include "core/mouseevents.h"
#include "core/windowevents.h"
#include "core/application.h"
#include "core/renderer.h"

LayerTexture::LayerTexture() :
    m_vbo({
        -0.5f, -0.5f, 0.0f, 0.0f,
        +0.5f, -0.5f, 1.0f, 0.0f,
        +0.5f, +0.5f, 1.0f, 1.0f,
        -0.5f, +0.5f, 0.0f, 1.0f
    }),
    m_texture("../res/textures/items.png"), m_mousePos(0.0f, 0.0f),
    m_camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 8.0f, 0.0051f, 250.0f, glm::radians(90.0f), glm::radians(0.0f)),
    m_proj(glm::perspective(glm::pi<float>() / 4.0f, Core::Application::get().framebufferSize().x / Core::Application::get().framebufferSize().y, 0.005f, 250.0f)) {

    m_ibo.bufferData({
        0, 1, 2,
        0, 2, 3
    });

    m_vao.bind();
    m_vbo.bind();
    m_ibo.bind();

    m_layout.pushFloats(2);
    m_layout.pushFloats(2);

    m_vao.addBuffer(m_vbo, m_layout);

    m_program.addShaderFromFile(Core::ShaderType::Vertex, "../res/shaders/vertexShader.glsl");
    m_program.addShaderFromFile(Core::ShaderType::Fragment, "../res/shaders/fragmentShader.glsl");
    m_program.link();

    m_texture.bind(0);
    m_program.bind();
    m_program.setUniform1i("u_Texture", 0 /*slot*/);

    // unbind the vao *before* the vbo or ibo
    m_vao.unbind();
    m_vbo.unbind();
    m_ibo.unbind();
    m_program.unbind();
}

void LayerTexture::onUpdate(float /*deltaTime*/) {
    m_blue += m_increment;
    if (m_blue > 1.0f) {
        m_increment = -0.01f;
    } else if (m_blue < 0.0f) {
        m_increment = 0.01f;
    }
}

void LayerTexture::onRender() {
    m_program.bind();
    m_program.setUniform4f("u_color", 0.2f, 0.8f, m_blue, 1.0f);
    if (m_uniformsDirty) {
        glm::mat4 view = m_camera.getViewMatrix();
        glm::mat4 model(1); //identity
        model = glm::translate(model, m_translation);
        glm::mat4 mvp = m_proj * view * model;
        m_program.setUniformMat4f("u_mvp", mvp);
        m_uniformsDirty = false;
    }
    m_program.unbind();
    Core::Renderer::draw(m_vao, m_ibo, m_program);
}

void LayerTexture::onImGuiRender() {
    if (ImGui::Button("Edit mode")) {
        this->swapLayer();
    }
    if (ImGui::DragFloat3("World Translation", &m_translation.x, 0.02f, -10.0f, 10.0f)) {
        m_uniformsDirty = true;
    }
}

void LayerTexture::onEvent(Core::Event& event) {
    Core::EventDispatcher dispatcher(event);
    dispatcher.dispatch<Core::MouseMovedEvent>([this](Core::MouseMovedEvent& e) { return this->onMouseMovedEvent(e); });
    dispatcher.dispatch<Core::MouseButtonPressedEvent>([this](Core::MouseButtonPressedEvent& e) { return this->onMousePressedEvent(e); });
    dispatcher.dispatch<Core::MouseButtonReleasedEvent>([this](Core::MouseButtonReleasedEvent& e) { return this->onMouseReleasedEvent(e); });
    dispatcher.dispatch<Core::MouseScrolledEvent>([this](Core::MouseScrolledEvent& e) { return this->onMouseScrolledEvent(e); });
    dispatcher.dispatch<Core::WindowResizedEvent>([this](Core::WindowResizedEvent& e) { return this->onWindowResizedEvent(e); });
}

bool LayerTexture::onMousePressedEvent(Core::MouseButtonPressedEvent& event) {
    if (event.getMouseButton() == GLFW_MOUSE_BUTTON_1) {
        m_leftMousePressed = true;
        m_mousePos = Core::Application::get().window()->mousePos();
        return true;
    }
    if (event.getMouseButton() == GLFW_MOUSE_BUTTON_2) {
        m_rightMousePressed = true;
        m_mousePos = Core::Application::get().window()->mousePos();
        return true;
    }
    if (event.getMouseButton() == GLFW_MOUSE_BUTTON_3) {
        m_camera.reset(glm::vec3(0.0f, 0.0f, 0.0f), 8.0f, glm::radians(90.0f), glm::radians(0.0f));
        m_uniformsDirty = true;
        return true;
    }
    return false;
}

bool LayerTexture::onMouseMovedEvent(Core::MouseMovedEvent& event) {
    //compute rotations
    double dx = event.x() - m_mousePos.x;
    double dy = event.y() - m_mousePos.y;

    m_mousePos.x = event.x();
    m_mousePos.y = event.y();

    if (m_leftMousePressed) {
        glm::vec2 size = Core::Application::get().framebufferSize();
        m_camera.rotateAzimuth(static_cast<float>(dx / size.x * 4.0));
        m_camera.rotatePolar(static_cast<float>(dy / size.y * 2.0));
        m_uniformsDirty = true;
    }

    if (m_rightMousePressed) {
        m_camera.moveHorizontal(static_cast<float>(-dx) / 10.0f);
        m_camera.moveVertical(static_cast<float>(dy) / 10.0f);
        m_uniformsDirty = true;
    }
    return false;
}

bool LayerTexture::onMouseReleasedEvent(Core::MouseButtonReleasedEvent& event) {
    if (event.getMouseButton() == GLFW_MOUSE_BUTTON_1) {
        m_leftMousePressed = false;
        return true;
    }
    if (event.getMouseButton() == GLFW_MOUSE_BUTTON_2) {
        m_rightMousePressed = false;
        return true;
    }
    return false;
}

bool LayerTexture::onWindowResizedEvent(Core::WindowResizedEvent& event) {
    m_proj = glm::perspective(glm::pi<float>() / 4.0f, static_cast<float>(event.width()) / static_cast<float>(event.height()), 0.005f, 250.0f);
    m_uniformsDirty = true;
    return false;
}

bool LayerTexture::onMouseScrolledEvent(Core::MouseScrolledEvent& event) {
    // compute new distance of camera from object
    float val = static_cast<float>(event.yOffset()) / 500.0f;

    if (val > 0.0f) {
        this->m_camera.zoom();
    } else {
        this->m_camera.dezoom();
    }

    m_uniformsDirty = true;
    return true;
}
