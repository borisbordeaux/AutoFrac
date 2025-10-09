#include <glm/detail/type_mat4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include "layertexture.h"
#include "imgui/imgui.h"

LayerTexture::LayerTexture() :
        m_vbo({ -0.5f, -0.5f, 0.0f, 0.0f,
                +0.5f, -0.5f, 1.0f, 0.0f,
                +0.5f, +0.5f, 1.0f, 1.0f,
                -0.5f, +0.5f, 0.0f, 1.0f }, 4 * 4 * sizeof(float)),
        m_ibo({ 0, 1, 2,
                0, 2, 3 }, 6),
        m_texture("../res/textures/items.png") {

    m_vao.bind();
    m_vbo.bind();
    m_ibo.bind();

    m_layout.pushFloats(2);
    m_layout.pushFloats(2);

    m_vao.addBuffer(m_vbo, m_layout);

    m_program.addShaderFromFile(ShaderType::Vertex, "../res/shaders/vertexShader.glsl");
    m_program.addShaderFromFile(ShaderType::Fragment, "../res/shaders/fragmentShader.glsl");
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
    glm::mat4 proj = glm::perspective(glm::pi<float>() / 4.0f, 1920.0f / 1080.0f, 0.005f, 250.0f);
    glm::mat4 view = glm::lookAt(m_cameraPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 model(1); //identity
    model = glm::translate(model, m_translation);
    glm::mat4 mvp = proj * view * model;

    m_program.bind();
    m_program.setUniform4f("u_color", 0.2f, 0.8f, m_blue, 1.0f);
    m_program.setUniformMat4f("u_mvp", mvp);
    m_program.unbind();

    m_renderer.draw(m_vao, m_ibo, m_program);
}

void LayerTexture::onImGuiRender() {
    ImGui::DragFloat3("Camera Position", &m_cameraPos.r, 0.02f, -10.0f, 10.0f);
    ImGui::DragFloat3("World Translation", &m_translation.r, 0.02f, -10.0f, 10.0f);
}
