#include "core/renderer.h"
#include <iostream>
#include <iomanip>

#include "core/vertexarray.h"
#include "core/shader.h"
#include "core/indexbuffer.h"
#include "core/log.h"

namespace Core {

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

void Renderer::draw(VertexArray const& vao, IndexBuffer const& ibo, ShaderProgram const& program) {
    program.bind();
    vao.bind();
    GLCall(glDrawElements(GL_TRIANGLES, ibo.count(), GL_UNSIGNED_INT, nullptr));
    program.unbind();
    vao.unbind();
}

void Renderer::draw(const VertexArray& vao, unsigned int count, const ShaderProgram& program) {
    program.bind();
    vao.bind();
    GLCall(glDrawArrays(GL_TRIANGLES, 0, count));
    program.unbind();
    vao.unbind();
}

void Renderer::clear() {
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Renderer::setClearColor(float r, float g, float b, float a) {
    GLCall(glClearColor(r, g, b, a));
}

void Renderer::initOpenGL() {
    if (glewInit() != GLEW_OK)
        LOG_ERROR("Glew not initialized");
    else {
        GLCall(const unsigned char* version = glGetString(GL_VERSION));
        LOG_INFO("OpenGL version is {}", version);
    }

    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    GLCall(glEnable(GL_DEPTH_TEST));

    glEnable(GL_PROGRAM_POINT_SIZE);
}

} // Core