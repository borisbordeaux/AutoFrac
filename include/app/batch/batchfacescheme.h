#ifndef AUTOFRAC_BATCHFACESCHEME_H
#define AUTOFRAC_BATCHFACESCHEME_H
#include "app/fractal/face.h"
#include "core/indexbuffer.h"
#include "core/shader.h"
#include "core/vertexarray.h"
#include "core/vertexbuffer.h"
#include "core/vertexbufferlayout.h"

namespace frac {
class Structure;
}

namespace Core {
class Camera;
}

class BatchFaceScheme {
public:
    BatchFaceScheme();
    void setMVP(const Core::Camera& camera, const glm::mat4& proj);
    void setStructure(const frac::Structure& structure);
    void setSelectedFace(std::size_t index);
    void render() const;

private:
    static std::vector<glm::vec2> tessellateFace(const frac::Structure& structure, std::size_t indexFace);
    void addFace(const std::vector<glm::vec2>& vertices, bool selected);
    void addVertexFace(const glm::vec2& v, float selected);
    static bool isValidTriangle(std::size_t indexPrev, std::size_t indexCurrent, std::size_t indexNext, const std::vector<glm::vec2>& vertices);
    static float cross2D(const glm::vec2& u, const glm::vec2& v);
    static bool pointInTriangle(const glm::vec2& P, const glm::vec2& A, const glm::vec2& B, const glm::vec2& C);
    void triangle(const glm::vec2& pos1, const glm::vec2& pos2, const glm::vec2& pos3, bool selected);

private:
    Core::VertexArray m_vao;
    Core::VertexBuffer m_vbo;
    Core::VertexBufferLayout m_layout;
    Core::ShaderProgram m_program;
    std::vector<float> m_data;
    std::size_t m_count = 0;
    std::size_t m_floatsPerVertex = 3;
    std::size_t m_nbFaces = 0;
    std::size_t m_nbTriangles = 0;
    std::size_t m_selectedFace = 0;
};

#endif //AUTOFRAC_BATCHFACESCHEME_H