#include "app/bcifs/grid.h"

namespace BCIFS {

GridFigure::GridFigure(Figure paths, float k, float length) :
    m_paths(std::move(paths)), m_k(k), m_length(length) {}

GridFigure::GridFigure(std::initializer_list<Path> paths, float k, float length) :
    m_paths(std::move(paths)), m_k(k), m_length(length) {}

} // BCIFS
