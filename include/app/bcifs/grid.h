#ifndef AUTOFRAC_GRID_H
#define AUTOFRAC_GRID_H

#include "transition.h"

namespace BCIFS {

class GridFigure {
public:
    explicit GridFigure(Figure paths, float k = -1.0f, float length = -1.0f);
    GridFigure(std::initializer_list<Path> paths, float k = -1.0f, float length = -1.0f);
    const Figure& paths() const { return m_paths; }
    float k() const { return m_k; }
    float length() const { return m_length; }

private:
    Figure m_paths;
    float m_k;
    float m_length;
};

using Grid = std::vector<GridFigure>;

} // BCIFS

#endif //AUTOFRAC_GRID_H