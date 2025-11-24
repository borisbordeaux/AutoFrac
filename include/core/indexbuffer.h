#ifndef AUTOFRAC_INDEXBUFFER_H
#define AUTOFRAC_INDEXBUFFER_H

#include <vector>

namespace Core {

class IndexBuffer {
public:
    IndexBuffer(const unsigned int* data, unsigned int count);
    IndexBuffer(std::vector<unsigned int> const& data, unsigned int count);
    ~IndexBuffer();

    void bind() const;
    void unbind() const;

    inline unsigned int count() const { return m_count; }

private:
    unsigned int m_rendererID;
    unsigned int m_count;
};

} // Core

#endif //AUTOFRAC_INDEXBUFFER_H
