#include "core/vertexbuffer.h"
#include "core/renderer.h"

namespace Core {

VertexBuffer::VertexBuffer(void const* data, unsigned int size) :
        m_rendererID(0) {
    GLCall(glGenBuffers(1, &m_rendererID));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_rendererID));
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::VertexBuffer(std::vector<float> const& data, unsigned int size) : VertexBuffer(data.data(), size) {}

VertexBuffer::~VertexBuffer() {
    GLCall(glDeleteBuffers(1, &m_rendererID));
}

void VertexBuffer::bind() const {
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_rendererID));
}

void VertexBuffer::unbind() const {
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

} // Core