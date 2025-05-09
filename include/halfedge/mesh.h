#ifndef AUTOFRAC_HE_MESH_H
#define AUTOFRAC_HE_MESH_H

#include <QString>
#include <vector>

class QMatrix4x4;

namespace he {

class Vertex;

class HalfEdge;

class Face;

class Mesh {
public:
    Mesh() = default;
    Mesh(Mesh const& mesh) = delete;            //copy constructor
    Mesh(Mesh&& mesh) = default;                //move constructor
    Mesh& operator=(Mesh const& mesh) = delete; //copy assignment operator
    Mesh& operator=(Mesh&& mesh) = default;     //move assignment operator
    ~Mesh();                                    //destructeur

    [[nodiscard]] std::vector<he::Vertex*> const& vertices() const;
    [[nodiscard]] std::vector<he::HalfEdge*> const& halfEdges() const;
    [[nodiscard]] std::vector<he::HalfEdge*> const& halfEdgesNoTwin() const;
    [[nodiscard]] std::vector<he::Face*> const& faces() const;

    void append(he::Vertex* v);
    void append(he::HalfEdge* he, bool completeNotTwin = false);
    void append(he::Face* f);

    he::HalfEdge* findByName(QString const& name, bool useOtherHalfEdgesOfVertices = true);

    void updateHalfEdgeNotTwin();
    void updateOtherHalfEdges();

    void reset();

    [[nodiscard]] QString toString() const;

    std::optional<std::size_t> indexOf(he::Vertex* v) const;
    std::optional<std::size_t> indexOf(he::Face* v) const;

    void updateFloatPosFromDoublePos();
    void updateDoublePosFromFloatPos();

    he::Vertex* cutHalfEdge(he::HalfEdge* he);
    he::HalfEdge* cutFace(he::Face* face, he::Vertex* v1, he::Vertex* v2);
    void remove(he::Face* f);
    void remove(he::HalfEdge* he);
    void remove(he::Vertex* v);

    void transformMesh(QMatrix4x4 const& transform);

private:
    std::vector<he::Vertex*> m_vertices;
    std::vector<he::HalfEdge*> m_halfEdges;
    std::vector<he::HalfEdge*> m_halfEdgesNotTwin;
    std::vector<he::Face*> m_faces;
};

} // poly

#endif //AUTOFRAC_HE_MESH_H
