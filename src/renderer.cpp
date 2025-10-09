#include "renderer.h"
#include <iostream>
#include <iomanip>

#include "vertexarray.h"
#include "shader.h"
#include "indexbuffer.h"

void GLClearError() {
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line) {
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cout << "[OpenGL Error] (0x"
                  << std::setfill('0')
                  << std::setw(4)
                  << std::hex << error
                  << std::dec // remettre en base 10 pour éviter les surprises après
                  << "): " << function << " " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}

void Renderer::draw(VertexArray const& vao, IndexBuffer const& ibo, ShaderProgram const& program) const {
    program.bind();
    vao.bind();
    GLCall(glDrawElements(GL_TRIANGLES, ibo.count(), GL_UNSIGNED_INT, nullptr));
    program.unbind();
    vao.unbind();
}

void Renderer::clear() const {
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::setClearColor(float r, float g, float b, float a) const {
    GLCall(glClearColor(r, g, b, a));
}

void Renderer::initOpenGL() {
    if (glewInit() != GLEW_OK)
        std::cout << "error glew init!" << std::endl;
    else {
        GLCall(const unsigned char* version = glGetString(GL_VERSION));
        std::cout << version << std::endl;
    }

    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
}
