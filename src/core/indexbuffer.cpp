#include "core/indexbuffer.h"
#include "core/renderer.h"

namespace Core {

IndexBuffer::IndexBuffer() :
        m_rendererID(0), m_count(0) {
    GLCall(glGenBuffers(1, &m_rendererID));
}

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count) :
        m_rendererID(0), m_count(count) {
    GLCall(glGenBuffers(1, &m_rendererID));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::IndexBuffer(const std::vector<unsigned int>& data) : IndexBuffer(data.data(), data.size()) {}

IndexBuffer::~IndexBuffer() {
    GLCall(glDeleteBuffers(1, &m_rendererID));
}

void IndexBuffer::bind() const {
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID));
}

void IndexBuffer::unbind() const {
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

void IndexBuffer::bufferData(const std::vector<unsigned int>& data) {
    m_count = data.size();
    this->bind();
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_count * sizeof(unsigned int), data.data(), GL_STATIC_DRAW));
    this->unbind();
}

} // Core