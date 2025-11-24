#ifndef AUTOFRAC_VERTEXBUFFER_H
#define AUTOFRAC_VERTEXBUFFER_H

#include <vector>

namespace Core {

class VertexBuffer {
public:
    VertexBuffer(const void* data, unsigned int size);
    VertexBuffer(std::vector<float> const& data, unsigned int size);
    ~VertexBuffer();

    void bind() const;
    void unbind() const;

private:
    unsigned int m_rendererID;
};

} // Core

#endif //AUTOFRAC_VERTEXBUFFER_H
