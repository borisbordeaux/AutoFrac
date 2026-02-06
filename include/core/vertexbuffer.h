#ifndef AUTOFRAC_VERTEXBUFFER_H
#define AUTOFRAC_VERTEXBUFFER_H

#include <vector>

namespace Core {

class VertexBuffer {
public:
    VertexBuffer();
    VertexBuffer(const void* data, unsigned int size);
    explicit VertexBuffer(std::vector<float> const& data);
    ~VertexBuffer();
    void bind() const;
    void unbind() const;
    void bufferData(std::vector<float> const& data);

private:
    unsigned int m_rendererID;
};

} // Core

#endif //AUTOFRAC_VERTEXBUFFER_H
