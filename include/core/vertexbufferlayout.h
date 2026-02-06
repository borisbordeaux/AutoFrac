#ifndef AUTOFRAC_VERTEXBUFFERLAYOUT_H
#define AUTOFRAC_VERTEXBUFFERLAYOUT_H

#include <vector>
#include <GL/glew.h>

namespace Core {

struct VertexBufferElement {
    unsigned int type;
    unsigned int count;
    unsigned int normalized;

    VertexBufferElement(unsigned int type, unsigned int count, unsigned int normalized) : type(type), count(count), normalized(normalized) {}
};

class VertexBufferLayout {
public:
    VertexBufferLayout() : m_stride(0) {}
    void pushFloats(unsigned int count) {
        m_elements.emplace_back(GL_FLOAT, count, GL_FALSE);
        m_stride += count * sizeof(float);
    }
    std::vector<VertexBufferElement> const& elements() const { return m_elements; }
    unsigned int stride() const { return m_stride; }

private:
    std::vector<VertexBufferElement> m_elements;
    unsigned int m_stride;
};

} // Core

#endif //AUTOFRAC_VERTEXBUFFERLAYOUT_H
