#ifndef AUTOFRAC_BCIFSVERTEX_H
#define AUTOFRAC_BCIFSVERTEX_H

#include <glm/vec3.hpp>

namespace BCIFS {

class BcifsVertex {
public:
    BcifsVertex(glm::vec3 pos, glm::vec3 frontColor, glm::vec3 backColor);
    const glm::vec3& pos() const { return m_pos; }
    const glm::vec3& frontColor() const { return m_frontColor; }
    const glm::vec3& backColor() const { return m_backColor; }

private:
    glm::vec3 m_pos;
    glm::vec3 m_frontColor;
    glm::vec3 m_backColor;
};

} // BCIFS

#endif //AUTOFRAC_BCIFSVERTEX_H
