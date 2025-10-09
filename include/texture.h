#ifndef AUTOFRAC_TEXTURE_H
#define AUTOFRAC_TEXTURE_H

#include <string>

class Texture {
public:
    explicit Texture(std::string const& filepath);
    ~Texture();

    void bind(unsigned int slot = 0) const;
    void unbind() const;

private:
    unsigned int m_rendererID;
    std::string m_filePath;
    unsigned char* m_localBuffer;
    int m_width;
    int m_height;
    int m_bitsPerPixel;
};


#endif //AUTOFRAC_TEXTURE_H
