#include "app/bcifs/bcifsvertex.h"

#include <algorithm>

namespace BCIFS {

BcifsVertex::BcifsVertex(glm::vec3 pos, glm::vec3 frontColor, glm::vec3 backColor) :
    m_pos(std::move(pos)), m_frontColor(std::move(frontColor)), m_backColor(std::move(backColor)) {}

} // BCIFS
