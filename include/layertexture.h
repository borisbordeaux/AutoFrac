#ifndef AUTOFRAC_LAYERTEXTURE_H
#define AUTOFRAC_LAYERTEXTURE_H


#include <glm/detail/type_vec3.hpp>
#include "layer.h"
#include "vertexarray.h"
#include "vertexbuffer.h"
#include "vertexbufferlayout.h"
#include "indexbuffer.h"
#include "shader.h"
#include "texture.h"
#include "renderer.h"

class LayerTexture : public Layer {
public:
    LayerTexture();
    void onUpdate(float deltaTime) override;
    void onRender() override;
    void onImGuiRender() override;

private:
    VertexArray m_vao;
    VertexBuffer m_vbo;
    VertexBufferLayout m_layout;
    IndexBuffer m_ibo;
    ShaderProgram m_program;
    Texture m_texture;

    float m_blue = 1.0f;
    float m_increment = 0.01f;
    glm::vec3 m_translation = { 0.0f, 0.0f, 0.0f };
    glm::vec3 m_cameraPos = { 0.0f, 0.0f, 5.0f };
    Renderer m_renderer;
};


#endif //AUTOFRAC_LAYERTEXTURE_H
