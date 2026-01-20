#ifndef AUTOFRAC_INDEXBUFFER_H
#define AUTOFRAC_INDEXBUFFER_H

#include <vector>

namespace Core {

class IndexBuffer {
public:
    IndexBuffer();
    IndexBuffer(const unsigned int* data, unsigned int count);
    explicit IndexBuffer(const std::vector<unsigned int>& data);
    ~IndexBuffer();

    void bind() const;
    void unbind() const;

    inline unsigned int count() const { return m_count; }

    void bufferData(const std::vector<unsigned int>& data);

private:
    unsigned int m_rendererID;
    unsigned int m_count;
};

} // Core

#endif //AUTOFRAC_INDEXBUFFER_H
