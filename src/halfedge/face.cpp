#include <vector>
#include <QMatrix4x4>

#include "halfedge/face.h"
#include "halfedge/halfedge.h"
#include "halfedge/vertex.h"

he::Face::Face(QString name, he::HalfEdge* halfEdge) : m_name(std::move(name)), m_halfEdge(halfEdge) {}

he::HalfEdge* he::Face::halfEdge() const {
    return m_halfEdge;
}

void he::Face::setHalfEdge(he::HalfEdge* halfEdge) {
    m_halfEdge = halfEdge;
}

QString he::Face::name() const {
    return m_name;
}

QVector3D he::Face::computeNormal() const {
    //take 3 points of the face
    QVector3D p1 = this->halfEdge()->origin()->pos();
    QVector3D p2 = this->halfEdge()->next()->origin()->pos();
    QVector3D p3 = this->halfEdge()->next()->next()->origin()->pos();

    //then compute the normal of the vectors created using the taken points
    return QVector3D::normal(p2 - p1, p3 - p2);
}

he::Point3D he::Face::computeNormalD() const {
    //take 3 points of the face
    he::Point3D p1 = this->halfEdge()->origin()->posD();
    he::Point3D p2 = this->halfEdge()->next()->origin()->posD();
    he::Point3D p3 = this->halfEdge()->next()->next()->origin()->posD();

    //then compute the normal of the vectors created using the taken points
    he::Point3D normal = he::Point3D::crossProduct(p2 - p1, p3 - p2);
    normal.normalize();
    return normal;
}

std::size_t he::Face::nbEdges() const {
    std::size_t res = 0;

    he::HalfEdge* he = this->m_halfEdge;
    he::HalfEdge* heNxt = he;
    do {
        res++;
        heNxt = heNxt->next();
    } while (heNxt != he);

    return res;
}

std::vector<he::HalfEdge*> he::Face::allHalfEdges() const {
    std::vector<he::HalfEdge*> res;

    he::HalfEdge* he = this->m_halfEdge;
    he::HalfEdge* heNxt = he;
    do {
        res.push_back(heNxt);
        heNxt = heNxt->next();
    } while (heNxt != he);

    return res;
}

std::vector<he::Vertex*> he::Face::allVertices() const {
    std::vector<he::Vertex*> res;

    for (he::HalfEdge* he: this->allHalfEdges()) {
        res.push_back(he->origin());
    }

    return res;
}

float he::Face::area() {
    // compute the new basis
    QVector3D axeX = (m_halfEdge->next()->origin()->pos() - m_halfEdge->origin()->pos()).normalized();
    QVector3D axeZ = this->computeNormal();
    QVector3D axeY = QVector3D::crossProduct(axeZ, axeX);

    // row major order
    QMatrix4x4 invTransMat = QMatrix4x4(axeX.x(), axeY.x(), axeZ.x(), 0,
                                        axeX.y(), axeY.y(), axeZ.y(), 0,
                                        axeX.z(), axeY.z(), axeZ.z(), 0,
                                        0, 0, 0, 1).inverted();

    float res = 0.0f;
    he::HalfEdge* he = this->m_halfEdge;
    he::HalfEdge* heNxt = he;
    do {
        QVector4D p1 = invTransMat * QVector4D(heNxt->origin()->pos(), 1.0f);
        QVector4D p2 = invTransMat * QVector4D(heNxt->next()->origin()->pos(), 1.0f);
        res += p1.x() * p2.y() - p2.x() * p1.y();
        heNxt = heNxt->next();
    } while (heNxt != he);

    return res / 2.0f;
}

QVector3D he::Face::barycenter() const {
    QVector3D res(0.0f, 0.0f, 0.0f);
    std::vector<he::HalfEdge*> halfedges = this->allHalfEdges();
    for (he::HalfEdge* he: halfedges) {
        res += he->origin()->pos();
    }
    res /= static_cast<float>(halfedges.size());
    return res;
}

QString he::Face::toString() const {
    QString res;
    res += m_name + ": has the halfedge " + (m_halfEdge ? m_halfEdge->name() : "nullptr") + " User data: " + m_userData;
    return res;
}

QString he::Face::userData() const {
    return m_userData;
}

void he::Face::setUserData(QString const& data) {
    m_userData = data;
}

he::Point3D he::Face::computePolar() const {
    // polar reciprocation : https://en.wikipedia.org/wiki/Dual_polyhedron#Polar_reciprocation
    he::Point3D n = this->computeNormalD();
    he::Point3D p = this->halfEdge()->origin()->posD();
    double d = he::Point3D::dotProduct(n, p);
    n /= d;
    return n;
}
