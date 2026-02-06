#ifndef AUTOFRAC_RENDERER_H
#define AUTOFRAC_RENDERER_H

#include <GL/glew.h>

namespace Core {

// insert an error in assembly to break into this line in debug mode
#define ASSERT(x) if(!(x)) __asm__ volatile("int3")
#define GLCall(x) GLClearError(); x; ASSERT(Core::GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

class VertexArray;
class IndexBuffer;
class ShaderProgram;

class Renderer {
public:
    static void initOpenGL();
    static void setClearColor(float r, float g, float b, float a);
    static void clear();
    static void draw(VertexArray const& vao, IndexBuffer const& ibo, ShaderProgram const& program);
    static void draw(VertexArray const& vao, unsigned int count, ShaderProgram const& program);
};

} // Core

#endif //AUTOFRAC_RENDERER_H
