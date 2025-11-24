#include "core/vertexarray.h"
#include "core/vertexbuffer.h"
#include "core/vertexbufferlayout.h"
#include "core/renderer.h"

namespace Core {

VertexArray::VertexArray() :
        m_rendererID(0) {
    GLCall(glGenVertexArrays(1, &m_rendererID));
}

VertexArray::~VertexArray() {
    GLCall(glDeleteVertexArrays(1, &m_rendererID));
}

void VertexArray::addBuffer(VertexBuffer const& vb, VertexBufferLayout const& layout) const {
    this->bind();
    vb.bind();
    std::vector<VertexBufferElement> const& elements = layout.elements();
    unsigned int offset = 0;
    for (unsigned int i = 0; i < elements.size(); i++) {
        VertexBufferElement const& element = elements[i];
        GLCall(glEnableVertexAttribArray(i));
        GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.stride(), reinterpret_cast<const void*>(offset)));
        offset += element.count * sizeof(float);
    }
}

void VertexArray::bind() const {
    GLCall(glBindVertexArray(m_rendererID));
}

void VertexArray::unbind() const {
    GLCall(glBindVertexArray(0));
}

} // Core