#ifndef AUTOFRAC_VERTEXARRAY_H
#define AUTOFRAC_VERTEXARRAY_H

namespace Core {

class VertexBuffer;

class VertexBufferLayout;

class VertexArray {
public:
    VertexArray();
    ~VertexArray();

    void addBuffer(VertexBuffer const& vb, VertexBufferLayout const& layout) const;

    void bind() const;
    void unbind() const;
private:
    unsigned int m_rendererID;
};

} // Core

#endif //AUTOFRAC_VERTEXARRAY_H
