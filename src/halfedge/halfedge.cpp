#include "halfedge/halfedge.h"
#include "halfedge/face.h"
#include "halfedge/vertex.h"

he::HalfEdge::HalfEdge(he::Vertex* origin, QString name) :
        m_origin(origin), m_face(nullptr),
        m_twin(nullptr), m_prev(nullptr),
        m_next(nullptr), m_name(std::move(name)),
        m_userData() {

}

he::Vertex* he::HalfEdge::origin() {
    return m_origin;
}

void he::HalfEdge::setOrigin(he::Vertex* origin) {
    m_origin = origin;
}

he::Face* he::HalfEdge::face() const {
    return m_face;
}

void he::HalfEdge::setFace(he::Face* face) {
    m_face = face;
}

he::HalfEdge* he::HalfEdge::twin() const {
    return m_twin;
}

void he::HalfEdge::setTwin(he::HalfEdge* twin) {
    m_twin = twin;
}

he::HalfEdge* he::HalfEdge::prev() {
    return m_prev;
}

void he::HalfEdge::setPrev(HalfEdge* prev) {
    m_prev = prev;
}

he::HalfEdge* he::HalfEdge::next() {
    return m_next;
}

void he::HalfEdge::setNext(he::HalfEdge* next) {
    m_next = next;
}

QString he::HalfEdge::name() const {
    return m_name;
}

void he::HalfEdge::setName(QString const& name) {
    m_name = name;
}

float he::HalfEdge::length() const {
    return (m_next->origin()->pos() - m_origin->pos()).length();
}

QString he::HalfEdge::toString() const {
    QString res = m_name;
    res += ": Origin: " + ((m_origin) ? m_origin->name() : "nullptr");
    res += " Face: " + ((m_face) ? m_face->name() : "nullptr");
    res += " Twin: " + ((m_twin) ? m_twin->name() : "nullptr");
    res += " Prev: " + ((m_prev) ? m_prev->name() : "nullptr");
    res += " Next: " + ((m_next) ? m_next->name() : "nullptr");
    res += " User data: " + m_userData;
    return res;
}

QString he::HalfEdge::userData() const {
    return m_userData;
}

void he::HalfEdge::setUserData(QString const& data) {
    m_userData = data;
}
